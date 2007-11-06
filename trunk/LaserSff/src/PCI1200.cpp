#include "PCI1200.h"

using namespace rcam;

#define SIMU 0

PCI1200::PCI1200()
{
    m_device = 1;

}

bool PCI1200::Init()
{
    i16 channel = 0;
    i16 polarity = 1;
    i16 voltref = 0;
    i16 updatemode = 0;
    i16 st;
#if !SIMU
    st = AO_Configure(m_device, channel, polarity, voltref, 0, updatemode);
    if(st != 0) {
        HandleError(st);
        return false;
    }

    st = DIG_Prt_Config(m_device, 2, 0, 1);
    if(st != 0) {
        HandleError(st);
        return false;
    }

    st = DIG_Prt_Config(m_device, 0, 0, 0);
    st = DIG_Prt_Config(m_device, 1, 0, 1);
    st = DIG_Out_Line(m_device, 1, 0, 0);
    st = DIG_Out_Line(m_device, 1, 1, 0);

#endif
    return true;
}

PCI1200::~PCI1200()
{

}

int PCI1200::ReadPort(i16 port)
{
    i16 line = 0;
#if !SIMU
    const int N = 10;
    const double THRESHOLD = 0.4;

    double sum = 0;


    for(int i = 0; i < N; i++) {
        DIG_In_Line(m_device, 0, port, &line);
        sum += line;
    }

    sum = sum / N;

    if(sum < THRESHOLD) {
        line = 0;
    } else {
        line = 1;
    }


#endif
    return line;
}

int PCI1200::ReadFinishSignal()
{
    return ReadPort(1);        
}

int PCI1200::ReadShutterSignal()
{
    return ReadPort(0);
}

bool PCI1200::SetLaserPower(int percent)
{
    f64 volt = percent * 0.05;
#if !SIMU
    return AO_VWrite(m_device, 0, volt) == 0;
#else
    return true;
#endif

}

void PCI1200::HandleError(i16 code)
{
#if !SIMU
    NIDAQErrorHandler(code, "PCI1200", 0);
#endif
}