#include "FileTextEdit.h"

#include <QFile>

FileTextEdit::FileTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
}

QString FileTextEdit::filePath() const
{
    return m_filePath;
}

void FileTextEdit::setFilePath(const QString &newFilePath)
{
    if (m_filePath == newFilePath)
        return;
    m_filePath = newFilePath;
    emit filePathChanged();
}

void FileTextEdit::openFile()
{
    if (filePath().isEmpty())
    {
        emit errorOccurred(tr("No file path set"));
        return;
    }
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly))
    {
        emit errorOccurred(tr("Could not open file"));
        return;
    }
    QTextStream in(&file);
    setPlainText(in.readAll());
    file.close();
}

void FileTextEdit::saveFile()
{
    if (filePath().isEmpty())
    {
        emit errorOccurred(tr("No file path set"));
        return;
    }
    QFile file(filePath());
    if (!file.open(QIODevice::WriteOnly))
    {
        emit errorOccurred(tr("Could not open file"));
        return;
    }
    QTextStream out(&file);
    out << toPlainText();
    file.close();
}

bool FileTextEdit::edited() const
{
    return m_edited;
}
