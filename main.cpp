#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <tcpserver.h>
#include <udpServer.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/I/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    qmlRegisterType<tcpSeverer>("tcpsever", 1, 0, "Tcpsever");
    qmlRegisterType<Server>("udpsever", 1, 0, "Udpserver");

    engine.load(url);

    return app.exec();
}
