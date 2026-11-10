#pragma once;
#include <QByteArray>
#include "CanManager.h"

class IsoTp{
    public:
    enum FrameType{
        SingleFrame=0x0,
        FirstFrame=0x1,
        ConsecutiveFrame=0x2,
        FlowControl=0x3
    };

    explicit IsoTp(CanManager *canManager);

    bool transmitAndReceive(uint32_t txId,const QByteArray &requestData,QByteArray &outResponseData);

   private:
   CanManager *m_canManager;
   bool sendFlowControl(uint32_t targetId); 
};