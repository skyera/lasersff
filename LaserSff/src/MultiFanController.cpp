#include "MultiFabController.h"
#include "FrameGrabberFactory.h"
#include "LaserFactory.h"
#include "PCI1200.h"
#include <wx/msgdlg.h>
#include <wx/config.h>
#include "Parameters.h"
#include <boost/scoped_ptr.hpp>
#include "ImageThread.h"
#include "DaqBoardFactory.h"
#include <wx/datetime.h>
#include <wx/dir.h>

using namespace rcam;
using namespace std;

MultiFabController::MultiFabController():m_estop(false)
{

}

MultiFabController::~MultiFabController()
{

}

bool MultiFabController::Init(int port)
{
    // laser
    bool ok1 = InitLaser(port);
    if(!ok1) {
        wxLogWarning("cannot init laser");
    }
    
    bool ok2 = InitDaqboard();
    if(!ok2) {
        wxMessageBox("cannot initialize DaqBoard", "error", wxOK|wxICON_WARNING);     
        
    }

    bool ok3 = InitFrameGrabber();
    if(!ok3) {
        wxMessageBox("cannot initialize frame grabber", "error", wxOK|wxICON_WARNING);     
    }
    
    m_init = ok1 && ok2 && ok3;
    m_init = true;
    m_laserPtr->SetDaqboard(m_daqboard);

    return m_init;
}

bool MultiFabController::InitLaser(int port)
{
    m_laserPtr = LaserFactory::CreateLaser("Yag");

    if(!m_laserPtr->Connect(port)) {
        wxString msg;
        msg << "Cannot open serial port " << port;
        //wxMessageBox(msg, "Error", wxOK|wxICON_ERROR);
        return false;
    } else {
  //      m_laserPtr->SetEPCOn();
        // write it to config file
        boost::scoped_ptr<wxConfig> config(new wxConfig(Parameters::AppName));
        config->Write(Parameters::SerialPort, port);

        return true;
    }
}

bool MultiFabController::InitDaqboard()
{
    m_daqboard = DaqBoardFactory::CreateDaqboard("pci1200");
    bool ok = m_daqboard->Init();
    return ok;
}

bool MultiFabController::Disconnect()
{
    if(m_laserPtr.get()) {
        m_laserPtr->Disconnect();
    }
    
    m_init = false;
    return true;
}

bool MultiFabController::MonitorLaser()
{
    int prevshutter = SHUTTER_CLOSE;
    int prevfinish = NOT_FINISHED;
    int shutter = SHUTTER_CLOSE;
    int finish = NOT_FINISHED;
    
    m_estop = false;
    while(!m_estop && (finish == NOT_FINISHED)) {
        finish = m_daqboard->ReadFinishSignal();
        shutter = m_daqboard->ReadShutterSignal();
        
        // shutter
        if(prevshutter == SHUTTER_CLOSE  && shutter == SHUTTER_OPEN) {
            m_laserPtr->OpenShutter();
        } else if(prevshutter == SHUTTER_OPEN && shutter == SHUTTER_CLOSE) {
            m_laserPtr->CloseShutter();
        } else {
            // do nothing
        }
        prevshutter = shutter;
        prevfinish = finish;
    }
    m_laserPtr->CloseShutter();
    m_laserPtr->SetLaserStandby();
    return true;
}

boost::shared_ptr<Laser> MultiFabController::GetLaser()
{
    return m_laserPtr;
}

bool MultiFabController::EmergencyStop()
{
    m_estop = true;
    return true;
}

bool MultiFabController::StartGrabImage(wxWindow* displayWindow, 
                                        wxCheckBox* binaryImage, 
                                        wxSlider *threshold,
                                        wxCheckBox *saveImage,
                                        wxTextCtrl *maxNumImage,
                                        wxTextCtrl *imagePath,
                                        wxTextCtrl *numSavedImage)
{
    m_displayWindow = displayWindow;
    m_binaryImageCheckBox = binaryImage;
    m_thresholdSlider = threshold;
    m_saveImageCheckBox = saveImage;
    m_maxNumImageText = maxNumImage;
    m_imagePathText = imagePath;
    m_numSavedImageText = numSavedImage;

    bool ok = m_frameGrabber->Init();
    if(!ok) {
        return false;
    } 

    // thread
    ImageThread *thread = new ImageThread(this);
    if(thread->Create() != wxTHREAD_NO_ERROR) {
        return false;
    }

    thread->Run();
    return true;
}

bool MultiFabController::InitFrameGrabber()
{
    m_frameGrabber = FrameGrabberFactory::CreateFrameGrabber("ni1402");   
    return true;
}

// image thread
void MultiFabController::DoAcquireImage()
{
    m_acquireImage = true;
    int count = 0;
    string path = CreateDirectory(m_imagePathText->GetValue().c_str());
    HWND hwnd = static_cast<HWND>(m_displayWindow->GetHandle());
    long max;
    m_maxNumImageText->GetValue().ToLong(&max);

    while(m_acquireImage) {
        m_frameGrabber->AcquireImage();
        
        // save
        if(m_saveImageCheckBox->GetValue() && count < max) {
            wxString filename;
            filename << path.c_str() << "\\" << count << ".bmp";
            m_frameGrabber->SaveImage(string(filename.c_str()));

            wxString numstr;
            numstr << count;
            m_numSavedImageText->SetLabel(numstr);
            count++;
        }
        
        // binary
        if(m_binaryImageCheckBox->GetValue()) {
            int threshold = m_thresholdSlider->GetValue();
            m_frameGrabber->BinaryImage(threshold);
        } 
        
        m_frameGrabber->DisplayImage(hwnd);
    }
    m_frameGrabber->Close();
}

bool MultiFabController::StopGrabImage()
{
    m_acquireImage = false;
    return true;
}

string MultiFabController::CreateDirectory(const string& rootpath)
{
    if(!::wxDirExists(rootpath.c_str())) {
        throw "file not exist";
    }

    wxDateTime now = wxDateTime::Now();
    int year = now.GetYear();
    int month = now.GetMonth();
    int day = now.GetDay();
    int hour = now.GetHour();
    int min = now.GetMinute();
    int sec = now.GetSecond();
    
    wxString path;
    path << year << "-" << month << "-" << day << "-" << hour << "-" << min << "-" << sec;
    path = wxString(rootpath.c_str()) + "\\" + path;
    bool ok = ::wxMkDir(path);
    if(!ok) {
  //      throw "cannot create directory";
    }
    return path.c_str();
}