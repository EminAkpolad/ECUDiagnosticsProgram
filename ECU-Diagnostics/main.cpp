
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QQmlContext>

class AppController :public QObject{
    Q_OBJECT

    public:
    explicit AppController(QObject *parent=nullptr):QObject(parent){}

    Q_INVOKABLE void merhabaYazdir(){
        qDebug()<< "Merhaba Akpolad";
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    AppController controller;

    engine.rootContext()->setContextProperty("appController", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            qCritical() << "HATA: QML penceresi oluşturulamadi!";
            QTimer::singleShot(0, QCoreApplication::instance(), []() {
                QCoreApplication::exit(-1);
            });
        },
        Qt::QueuedConnection
    );

    qDebug() << "ECU Diagnostics baslatiliyor...";

    engine.loadFromModule("ECUDiagnostics", "Main");

    qDebug() << "QML yukleme komutu tamamlandi.";

    return app.exec();
}
#include "main.moc"