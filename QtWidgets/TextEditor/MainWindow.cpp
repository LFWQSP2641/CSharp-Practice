#include "MainWindow.h"

#include "FileTextEdit.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // open file action
    QAction *openFileAction = new QAction(tr("&Open..."), this);
    openFileAction->setShortcuts(QKeySequence::Open);
    openFileAction->setStatusTip(tr("Open an existing file"));
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);

    // new file action
    QAction *newFileAction = new QAction(tr("&New"), this);
    newFileAction->setShortcuts(QKeySequence::New);
    newFileAction->setStatusTip(tr("Create a new file"));
    connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);

    // save file action
    QAction *saveFileAction = new QAction(tr("&Save"), this);
    saveFileAction->setShortcuts(QKeySequence::Save);
    saveFileAction->setStatusTip(tr("Save the document to disk"));
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);

    // save as file action
    QAction *saveAsFileAction = new QAction(tr("Save &As..."), this);
    saveAsFileAction->setShortcuts(QKeySequence::SaveAs);
    saveAsFileAction->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsFileAction, &QAction::triggered, this, &MainWindow::saveAsFile);

    // close file action
    QAction *closeFileAction = new QAction(tr("&Close"), this);
    closeFileAction->setShortcuts(QKeySequence::Close);
    closeFileAction->setStatusTip(tr("Close this window"));
    connect(closeFileAction, &QAction::triggered, this, &MainWindow::closeFile);

    m_fileActions << openFileAction << saveFileAction << saveAsFileAction << closeFileAction;

    // undo action
    QAction *undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setStatusTip(tr("Undo the last action"));
    undoAction->setEnabled(false);
    connect(undoAction, &QAction::triggered, this, &MainWindow::textEditUndo);

    // resume action
    QAction *resumeAction = new QAction(tr("&Resume"), this);
    resumeAction->setShortcuts(QKeySequence::Redo);
    resumeAction->setStatusTip(tr("Resume the last action"));
    resumeAction->setEnabled(false);
    connect(resumeAction, &QAction::triggered, this, &MainWindow::textEditResume);

    // separator
    QAction *separatorAction = new QAction(this);
    separatorAction->setSeparator(true);

    // find action
    QAction *findAction = new QAction(tr("&Find"), this);
    findAction->setShortcuts(QKeySequence::Find);
    findAction->setStatusTip(tr("Find text"));
    findAction->setEnabled(false);
    connect(findAction, &QAction::triggered, this, &MainWindow::textEditFind);

    m_editActions << undoAction << resumeAction << separatorAction << findAction;

    // preferences action
    QAction *preferencesAction = new QAction(tr("&Preferences..."), this);
    preferencesAction->setStatusTip(tr("Change the application preferences"));
    connect(preferencesAction, &QAction::triggered, this, &MainWindow::preferences);

    m_preferencesActions << preferencesAction;

    // about action
    QAction *aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    m_aboutActions << aboutAction;

    ui->fileMenu->addActions(m_fileActions);
    ui->editMenu->addActions(m_editActions);
    ui->preferencesMenu->addActions(m_preferencesActions);
    ui->aboutMenu->addActions(m_aboutActions);
}

MainWindow::~MainWindow()
{
    delete ui;
}

FileTextEdit *MainWindow::getCurrentFileTextEdit() const
{
    return qobject_cast<FileTextEdit *>(ui->textEditTabWidget->currentWidget());
}

void MainWindow::openFile()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (filePath.isEmpty())
        return;
    createNewFileTextEdit(filePath);
}

void MainWindow::newFile()
{
    createNewFileTextEdit();
}

void MainWindow::saveFile()
{
    FileTextEdit *fileTextEdit = getCurrentFileTextEdit();
    if (fileTextEdit == nullptr)
        return;
    if (fileTextEdit->filePath().isEmpty())
    {
        saveAsFile();
    }
    else
    {
        fileTextEdit->saveFile();
    }
}

void MainWindow::saveAsFile()
{
    FileTextEdit *fileTextEdit = getCurrentFileTextEdit();
    if (fileTextEdit == nullptr)
        return;
    const QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"));
    if (filePath.isEmpty())
        return;
    fileTextEdit->setFilePath(filePath);
    fileTextEdit->saveFile();
}

void MainWindow::closeFile()
{
    FileTextEdit *fileTextEdit = getCurrentFileTextEdit();
    if (fileTextEdit == nullptr)
        return;
    if (!fileTextEdit->edited())
    {
        ui->textEditTabWidget->removeTab(ui->textEditTabWidget->currentIndex());
        fileTextEdit->deleteLater();
        return;
    }
    const QMessageBox::StandardButton result = QMessageBox::warning(this, tr("Unsaved Changes"),
                                                                    tr("This document has been modified. Do you want to save your changes?"),
                                                                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (result == QMessageBox::Save)
    {
        saveFile();
    }
    else if (result == QMessageBox::Cancel)
    {
        return;
    }
}

void MainWindow::preferences()
{
    QMessageBox::information(this, tr("Preferences"), tr("No preferences available"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Text Editor"), tr("The <b>Text Editor</b> example demonstrates how to create a simple text editor using Qt."));
}

void MainWindow::createNewFileTextEdit(std::optional<QString> filePath)
{
    FileTextEdit *fileTextEdit = new FileTextEdit(this);
    if (filePath.has_value())
    {
        fileTextEdit->setFilePath(filePath.value());
        fileTextEdit->openFile();
        const auto fileName = QFileInfo(filePath.value()).fileName();
        ui->textEditTabWidget->addTab(fileTextEdit, fileName);
        ui->textEditTabWidget->setTabToolTip(ui->textEditTabWidget->count() - 1, filePath.value());
    }
    else
    {
        ui->textEditTabWidget->addTab(fileTextEdit, tr("New File"));
    }
    ui->textEditTabWidget->setCurrentWidget(fileTextEdit);
}

void MainWindow::on_textEditTabWidget_tabCloseRequested(int index)
{
    closeFile();
}

void MainWindow::textEditUndo()
{
    FileTextEdit *fileTextEdit = getCurrentFileTextEdit();
    if (fileTextEdit == nullptr)
        return;
    fileTextEdit->undo();
}

void MainWindow::textEditResume()
{
    FileTextEdit *fileTextEdit = getCurrentFileTextEdit();
    if (fileTextEdit == nullptr)
        return;
    fileTextEdit->redo();
}

void MainWindow::textEditFind()
{
    FileTextEdit *fileTextEdit = getCurrentFileTextEdit();
    if (fileTextEdit == nullptr)
        return;

    auto findDialog = new QDialog(this);
    findDialog->setWindowTitle(tr("Find"));
    findDialog->setModal(false);
    findDialog->setAttribute(Qt::WA_DeleteOnClose);

    auto findButton = new QPushButton(tr("Find"), findDialog);
    auto cancelButton = new QPushButton(tr("Cancel"), findDialog);
    auto findLineEdit = new QLineEdit(findDialog);
    auto gridLayout = new QGridLayout(findDialog);

    gridLayout->addWidget(findLineEdit, 0, 0, 1, 2);
    gridLayout->addWidget(findButton, 1, 0);
    gridLayout->addWidget(cancelButton, 1, 1);

    connect(findButton, &QPushButton::clicked, [fileTextEdit, findLineEdit]
            {
        const QString text = findLineEdit->text();
        if (text.isEmpty())
            return;
        fileTextEdit->find(text); });

    connect(cancelButton, &QPushButton::clicked, findDialog, &QDialog::close);

    findDialog->show();
}

void MainWindow::on_textEditTabWidget_currentChanged(int index)
{
    if (index == -1)
        return;
    for (auto i : ui->editMenu->actions())
    {
        if (i->text() == tr("&Undo"))
        {
            i->setEnabled(true);
        }
        else if (i->text() == tr("&Resume"))
        {
            i->setEnabled(true);
        }
        else if (i->text() == tr("&Find"))
        {
            i->setEnabled(true);
        }
    }
}
