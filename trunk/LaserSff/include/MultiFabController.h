#ifndef RCAM_MULTIFAB_CONTROLLER_H
#define RCAM_MULTIFAB_CONTROLLER_H

#include <boost/shared_ptr.hpp>

#include "Controller.h"
#include "DaqBoard.h"
#include "FrameGrabber.h"
#include <string>

namespace rcam {

class Laser;

class MultiFabController: public Controller 
{
public:
    MultiFabController();
    ~MultiFabController();

    virtual bool Init(int port);
    virtual bool Disconnect();
    virtual bool MonitorLaser();
    virtual boost::shared_ptr<Laser> GetLaser();
    virtual bool EmergencyStop();
    virtual bool StartGrabImage(wxWindow* displayWindow, 
                                wxCheckBox* binaryImage, 
                                wxSlider *threshold,
                                wxCheckBox *saveImage,
                                wxTextCtrl *maxNumImage,
                                wxTextCtrl *imagePath,
                                wxTextCtrl *numSavedImage);
    virtual bool StopGrabImage();
    virtual bool IsDisplayingImage();
    virtual wxString GetImagePath();

    void DoAcquireImage();
    void Close();
private:

    bool InitLaser(int port);
    bool InitDaqboard();
    bool InitFrameGrabber();
    std::string CreateDirectory(const std::string& rootpath);

    boost::shared_ptr<Laser>  m_laserPtr;
    boost::shared_ptr<DaqBoard> m_daqboard;
    boost::shared_ptr<FrameGrabber> m_frameGrabber;

    bool m_estop;
    bool m_acquireImage;

    wxWindow* m_displayWindow;
    wxCheckBox* m_binaryImageCheckBox;
    wxSlider *m_thresholdSlider;
    wxCheckBox *m_saveImageCheckBox;
    wxTextCtrl *m_maxNumImageText;
    wxTextCtrl *m_imagePathText;
    wxTextCtrl *m_numSavedImageText;

    wxString m_imagePath;

};

}

#endif