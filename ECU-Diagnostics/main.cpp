
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QQmlContext>
#include <iostream>
#include "database/DatabaseManager.h"
using namespace std;

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
    DatabaseManager db;
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    AppController controller;

    engine.rootContext()->setContextProperty("appController", &controller);

    if(db.connect()){
        db.fetchMachineCategories();
    }
    else{
        cout<<"Error database not found"<<endl;
    }


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