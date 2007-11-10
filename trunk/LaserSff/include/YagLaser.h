#ifndef RCAM_YAGLASER_H
#define RCAM_YAGLASER_H

#include "Laser.h"
//#include <wx/ctb/iobase.h>
//#include <wx/ctb/serport.h>
#include "SerialPort.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include "PCI1200.h"

namespace rcam {

// YAG Laser 
class YagLaser: public Laser
{
public:
    YagLaser();
    ~YagLaser();

    virtual bool Connect(int port);
    virtual bool Disconnect();
    virtual bool OpenShutter();
    virtual bool CloseShutter();
    virtual bool IsShutterOpen();
    virtual bool IsLaserOn();
    
    bool SetLaserOn();
    bool SetLaserOff();
    bool SetLaserStandby();

    std::string GetLaserStatus();
    std::string GetShutterStatus();
    virtual std::string GetPowerPercent();
    virtual std::string GetPowerWatt();
    bool SetEPCOn();
    bool SetEPCOff();

    bool SingleShot();
    
    virtual bool SetPower(int percent);

    bool SetEPCHighValue();
    bool SetEPCLowValue();
    std::string GetEPCStatus();
    std::string GetInterlockStatus();
    virtual void SetDaqboard(const boost::shared_ptr<DaqBoard>& board);
private:
    std::string ReadResponse();
    void Write(const std::string& cmd);

    // data
    bool m_connected;
    boost::shared_ptr<SerialPort> m_serialPortPtr;
    boost::shared_ptr<DaqBoard> m_daqboard;
    bool m_shutterStatus;
    bool m_laserOn;
};

}

#endif