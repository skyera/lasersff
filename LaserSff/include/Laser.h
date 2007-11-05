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
    
    virtual std::string LaserStatus() = 0;
    virtual std::string ShutterStatus() = 0;
    virtual std::string PowerPercent() = 0;
    virtual std::string PowerWatt() = 0;
    virtual bool SingleShot() = 0;
    
    virtual bool SetLaserOn() = 0;
    virtual bool SetLaserOff() = 0;
    virtual bool SetLaserStandby() = 0;

    virtual bool SetPower(int percent) = 0;
    virtual std::string EPCStatus() = 0;
    virtual std::string InterlockStatus() = 0;
    virtual bool SetEPCOn() = 0;
    virtual void SetDaqboard(const boost::shared_ptr<DaqBoard>& board) = 0;

};

}

#endif
