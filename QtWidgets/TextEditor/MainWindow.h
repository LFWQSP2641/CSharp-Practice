#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
} // namespace Ui
QT_END_NAMESPACE
class FileTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    Ui::MainWindow *ui;

    QList<QAction *> m_fileActions;
    QList<QAction *> m_editActions;
    QList<QAction *> m_preferencesActions;
    QList<QAction *> m_aboutActions;

protected:
    FileTextEdit *getCurrentFileTextEdit() const;

protected slots:
    void openFile();
    void newFile();
    void saveFile();
    void saveAsFile();
    void closeFile();
    void preferences();
    void about();

    void textEditUndo();
    void textEditResume();
    void textEditFind();

private slots:
    void createNewFileTextEdit(std::optional<QString> filePath = std::nullopt);
    void on_textEditTabWidget_tabCloseRequested(int index);
    void on_textEditTabWidget_currentChanged(int index);
};
