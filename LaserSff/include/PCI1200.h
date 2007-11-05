#ifndef RCAM_PCI1200_H
#define RCAM_PCI1200_H

#include "DaqBoard.h"
#include <nidaqex.h>

namespace rcam {

class PCI1200: public DaqBoard
{
public:
    PCI1200();
    ~PCI1200();

    virtual bool Init();
    
    int ReadPort(i16 port);

    virtual int ReadShutterSignal();
    virtual int ReadFinishSignal();
    virtual bool SetLaserPower(int percent);
private:

    void HandleError(i16 code);
    i16 m_device;
};

}

#endif