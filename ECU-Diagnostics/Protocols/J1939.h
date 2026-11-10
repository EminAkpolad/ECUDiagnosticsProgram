#pragma once
#include "CanManager.h"
#include "IsoTp.h"
#include <QString>

class j1939Protocol{
    public:
    explicit j1939Protocol(CanManager *CanManager);

    QString requestChassisVIN();

    bool requestEngineMetrics(double &outRpm, uint32_t &outEngineHours);

    private:
    CanManager *m_canManager;
    IsoTp *m_isoTp;
}