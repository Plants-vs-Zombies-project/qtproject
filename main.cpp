#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <tcpclient.h>
#include <udpClient.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/II/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    qmlRegisterType<TcpClient>("tcpclient", 1, 0, "Tcpclient");
    qmlRegisterType<Client>("udpclient", 1, 0, "Udpclient");

    engine.load(url);

    return app.exec();
}
