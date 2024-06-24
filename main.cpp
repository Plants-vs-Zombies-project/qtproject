#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <friend.h>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/i/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    qmlRegisterType<Friend>("Friend", 1, 0, "Friend");
    engine.load(url);

    return app.exec();
}
