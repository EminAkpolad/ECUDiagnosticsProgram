#pragma once
#include <QObject>
#include <QByteArray>
#include <QVector>

struct CanFrame{
    uint32_t arbitrationId;
    QByteArray data;
    bool isExtended;
};

class CanManager : public QObject{
    Q_OBJECT
    public:
    explicit CanManager(QObject *parent=nullptr);
    ~CanManager();

    bool openChannel(const QString &deviceInterface);
    void closeChannel();
    bool sendFrame(const CanFrame &frame);

    signals:
    void frameReceived(const CanFrame &frame);


    private:
    void pollHardware();
    bool m_isConnected=false;
};