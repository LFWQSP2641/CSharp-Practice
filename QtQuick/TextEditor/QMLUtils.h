#pragma once

#include <QObject>

class QMLUtils : public QObject
{
    Q_OBJECT

public:
    explicit QMLUtils(QObject *parent = nullptr);

    Q_INVOKABLE QString readTextFile(const QUrl &url);
    Q_INVOKABLE void writeTextFile(const QUrl &url, const QString &text);
    Q_INVOKABLE QString getFileName(const QUrl &url);

signals:
};
