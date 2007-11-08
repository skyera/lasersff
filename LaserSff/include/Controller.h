#ifndef RCAM_CONTROLLER_H
#define RCAM_CONTROLLER_H

#include <wx/wx.h>
#include <boost/shared_ptr.hpp>

namespace rcam {

class Laser;

class Controller
{
public:
    Controller();
    virtual ~Controller();

    virtual bool Init(int port) = 0;
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

protected:
    bool m_init;
};

}

#endif