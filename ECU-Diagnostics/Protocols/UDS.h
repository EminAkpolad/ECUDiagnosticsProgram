#pragma once
#include <QByteArray>
#include <QVector>
#include <QString>
#include "IsoTp.h"

struct DiagnosticTroubleCode{
    QString dtcCode;
    uint8_t statusMask;
};

class UdsProtocol{
    public:
    explicit UdsProtocol(IsoTp *isoTp);

    bool setSessionMode(uint8_t sessionType);

    QVector<DiagnosticTroubleCode> readDTCs();

    QByteArray readDataByIdentifier(uint16_t dataIdentifier);

    private:
    IsoTp *m_isoTp;
}