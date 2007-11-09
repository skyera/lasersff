#ifndef RCAM_CONTROLLER_H
#define RCAM_CONTROLLER_H

#include <wx/wx.h>
#include <boost/shared_ptr.hpp>

class MainFrame;

namespace rcam {

class Laser;


class Controller
{
public:
    Controller();
    virtual ~Controller();

    virtual bool Init() = 0;
    virtual bool Disconnect() = 0;
    virtual bool MonitorLaser() = 0;
    virtual bool EmergencyStop() = 0;

    bool IsInited();

    virtual boost::shared_ptr<Laser> GetLaser() = 0;

    virtual bool StartGrabImage(wxWindow* displayWindow, 
                                wxCheckBox* binaryImage, 
                                wxSlider *threshold,
                                wxCheckBox *saveImage,
                                wxTextCtrl *maxNumImage,
                                wxTextCtrl *imagePath,
                                wxTextCtrl *numSavedImage) = 0;
    virtual bool StopGrabImage() = 0;
    virtual bool IsDisplayingImage() = 0;
    virtual wxString GetImagePath() = 0;
    virtual void SetFrame(MainFrame *frame) = 0;
    virtual void Close() = 0;
    virtual void CheckLaserStatus() = 0;
    virtual void SetLaserSerialPort(int port) = 0;
    virtual int GetLaserSerialPort() = 0;
    virtual bool OpenLaserShutter() = 0;
    virtual bool CloseLaserShutter() = 0;
    virtual bool DoSingleShot() = 0;
    virtual bool SetLaserPowerPercent(int percent) = 0;
    virtual bool SetLaserOn() = 0;
    virtual bool SetLaserOff() = 0;
    virtual bool SetLaserStandby() = 0;
    virtual bool SetLaserEpcOn() = 0;
    virtual void ChooseImagePath() = 0;
    virtual bool Run() = 0;

protected:
    bool m_init;
};

}

#endif