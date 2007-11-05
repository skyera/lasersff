#ifndef RCAM_YAGLASER_H
#define RCAM_YAGLASER_H

#include "Laser.h"
#include <wx/ctb/iobase.h>
#include <wx/ctb/serport.h>
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
    
    bool SetLaserOn();
    bool SetLaserOff();
    bool SetLaserStandby();

    std::string LaserStatus();
    std::string ShutterStatus();
    virtual std::string PowerPercent();
    virtual std::string PowerWatt();
    bool SetEPCOn();
    bool SetEPCOff();

    bool SingleShot();
    
    virtual bool SetPower(int percent);

    bool SetEPCHighValue();
    bool SetEPCLowValue();
    std::string EPCStatus();
    std::string InterlockStatus();
    virtual void SetDaqboard(const boost::shared_ptr<DaqBoard>& board);
private:
    std::string ReadResponse();
    void Write(const std::string& cmd);

    // data
    bool m_connected;
    boost::shared_ptr<wxSerialPort> m_serialPortPtr;
    boost::shared_ptr<DaqBoard> m_daqboard;
};

}

#endif