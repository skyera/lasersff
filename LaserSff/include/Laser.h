#ifndef RCAM_LASER_H
#define RCAM_LASER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "DaqBoard.h"

namespace rcam {

// Abstract laser class
class Laser
{
public:
    Laser();
    virtual ~Laser();
    
    virtual bool Connect(int port) = 0;
    virtual bool Disconnect() = 0;
    virtual bool OpenShutter() = 0;
    virtual bool CloseShutter() = 0;
    
    virtual std::string GetLaserStatus() = 0;
    virtual std::string GetShutterStatus() = 0;
    virtual std::string GetPowerPercent() = 0;
    virtual std::string GetPowerWatt() = 0;
    virtual bool SingleShot() = 0;
    
    virtual bool SetLaserOn() = 0;
    virtual bool SetLaserOff() = 0;
    virtual bool SetLaserStandby() = 0;

    virtual bool SetPower(int percent) = 0;
    virtual std::string GetEPCStatus() = 0;
    virtual std::string GetInterlockStatus() = 0;
    virtual bool SetEPCOn() = 0;
    virtual void SetDaqboard(const boost::shared_ptr<DaqBoard>& board) = 0;
    virtual bool IsShutterOpen() = 0;
    virtual bool IsLaserOn() = 0;
};

}

#endif
