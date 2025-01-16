#include "QMLUtils.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto qmlUtils(new QMLUtils(&app));

    qmlRegisterSingletonInstance("QMLUtils", 1, 0, "QMLUtils", qmlUtils);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("TextEditor", "Main");

    return app.exec();
}
