#pragma once

#include <QObject>
#include <QByteArray>
#include <QVector>
#include <QCanBusDevice>
#include <cstdint>

struct CanFrame {
    uint32_t arbitrationId;
    QByteArray data;
    bool isExtended;
};

class CanManager : public QObject {
    Q_OBJECT

public:
    explicit CanManager(QObject *parent = nullptr);
    ~CanManager() override;

    bool openChannel(const QString &deviceInterface, int bitrate = 250000);
    void closeChannel();
    bool sendFrame(const CanFrame &frame);

signals:
    void frameReceived(const CanFrame &frame);
    void errorOccurred(const QString &errorMessage); 

private slots:
    void onFramesReceived(); 
private:
    QCanBusDevice *m_canDevice = nullptr;
    bool m_isConnected = false;
};