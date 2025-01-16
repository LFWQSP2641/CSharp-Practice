#pragma once

#include <QTextEdit>

class FileTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit FileTextEdit(QWidget *parent = nullptr);

    QString filePath() const;
    void setFilePath(const QString &newFilePath);

    bool edited() const;

public slots:
    void openFile();
    void saveFile();

protected:
    QString m_filePath;
    bool m_edited = false;

signals:
    void filePathChanged();
    void errorOccurred(const QString &errorMessage);

private:
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged FINAL)
    Q_PROPERTY(bool edited READ edited CONSTANT FINAL)
};
