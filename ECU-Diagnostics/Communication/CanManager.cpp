#include "CanManager.h"
#include <QCanBus>
#include <QVariant>
#include <QDebug>

CanManager::CanManager(QObject *parent):QObject(parent){}

CanManager::~CanManager(){
    closeChannel();
}

bool CanManager::openChannel(const QString &deviceInterface,int bitrate){
    closeChannel();

    QString errorString;

    m_canDevice=QCanBus::instance()->createDevice("peakcan",deviceInterface,&errorString);

    if(!m_canDevice){
        emit errorOccurred("Cihaz Olusturulamadi: " + errorString);
        return false;
    }

    connect(m_canDevice, &QCanBusDevice::framesReceived,
        this, &CanManager::onFramesReceived);

    m_canDevice->setConfigurationParameter(QCanBusDevice::BitRateKey,QVariant::fromValue(bitrate));

    if(!m_canDevice->connectDevice()){
        emit errorOccurred("Hata Vaglanamadi : " + m_canDevice->errorString());
        delete m_canDevice;
        m_canDevice=nullptr;
        return false;
    }

    m_isConnected=true;
    return true;
}

void CanManager::closeChannel(){
    if(m_canDevice){
        if(m_canDevice->state()==QCanBusDevice::ConnectedState){
            m_canDevice->disconnectDevice();
        }
        delete m_canDevice;
        m_canDevice=nullptr;
    }
    m_isConnected=false;
}

void CanManager::onFramesReceived(){
    if(!m_canDevice) return;

    while(m_canDevice->framesAvailable()){
        const QCanBusFrame qtFrame=m_canDevice->readFrame();

        if(qtFrame.frameType()==QCanBusFrame::ErrorFrame){
            continue;
        }

        CanFrame myFrame;
        myFrame.arbitrationId=qtFrame.frameId();
        myFrame.data=qtFrame.payload();
        myFrame.isExtended=qtFrame.hasExtendedFrameFormat();

        emit frameReceived(myFrame);
    }
}

bool CanManager::sendFrame(const CanFrame &frame){
    if(!m_canDevice || !m_isConnected){
        return false;
    }

    QCanBusFrame qtFrame;
    qtFrame.setFrameId(frame.arbitrationId);
    qtFrame.setPayload(frame.data);
    qtFrame.setExtendedFrameFormat(frame.isExtended);

    return m_canDevice->writeFrame(qtFrame);
}