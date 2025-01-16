#include "QMLUtils.h"

#include <QFile>
#include <QTextStream>
#include <QUrl>

QMLUtils::QMLUtils(QObject *parent)
    : QObject{ parent }
{
}

QString QMLUtils::readTextFile(const QUrl &url)
{
    QFile file{ url.toLocalFile() };
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    QTextStream stream{ &file };
    return stream.readAll();
}

void QMLUtils::writeTextFile(const QUrl &url, const QString &text)
{
    QFile file{ url.toLocalFile() };
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream stream{ &file };
    stream << text;
}

QString QMLUtils::getFileName(const QUrl &url)
{
    return url.fileName();
}
