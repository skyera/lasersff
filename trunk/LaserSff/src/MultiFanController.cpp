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
#include "MainFrame.h"
#include "LaserThread.h"
#include <boost/lexical_cast.hpp>

using namespace rcam;
using namespace std;

MultiFabController::MultiFabController():m_estop(false), m_acquireImage(false)
{
}

MultiFabController::~MultiFabController()
{

}

bool MultiFabController::Init()
{
    boost::scoped_ptr<wxConfig> config(new wxConfig(Parameters::AppName));
    
    // image path
    wxString path = config->Read(Parameters::ImagePath, "c:\\");
    m_frame->SetImagePath(path);

    // power in spinctrl
    int power = config->Read(Parameters::PowerPercent, 51);
    m_frame->SetPowerSpin(power);

    // laser port
    int port = config->Read(Parameters::SerialPort, 3); 
    m_laserSerialPort = port;
    // laser
    bool ok1 = InitLaser(port);
    if(!ok1) {
        wxString msg;
        msg << "cannot open serial port " << port << " for laser";
        wxLogWarning(msg);
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
    m_laserPtr->SetDaqboard(m_daqboard);

    wxString info;
    if(m_init) {
        info << "laser is connected to serial port " << port;
    } else {
        info << "laser failed to connect to serial port " << port;
    }
    m_frame->UpdateUI(info, m_init);
    return m_init;
}

bool MultiFabController::InitLaser(int port)
{
    m_laserPtr = LaserFactory::CreateLaser("Yag");

    if(!m_laserPtr->Connect(port)) {
        return false;
    } else {
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
    m_frame->UpdateUI("laser is disconnected", false);

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
    while(!m_estop) {// && (finish == NOT_FINISHED)) {
        //finish = m_daqboard->ReadFinishSignal();
        shutter = m_daqboard->ReadShutterSignal();

        // shutter
        if(prevshutter == SHUTTER_CLOSE  && shutter == SHUTTER_OPEN) {
            this->OpenLaserShutter();
        } else if(prevshutter == SHUTTER_OPEN && shutter == SHUTTER_CLOSE) {
            this->CloseLaserShutter();
        } else {
            // do nothing
        }
        prevshutter = shutter;
        //prevfinish = finish;
    }
    this->CloseLaserShutter();
    this->SetLaserStandby();
    m_frame->FinishRun();
    return true;
}

boost::shared_ptr<Laser> MultiFabController::GetLaser()
{
    return m_laserPtr;
}

bool MultiFabController::EmergencyStop()
{
    m_estop = true;
    m_frame->FinishRun();
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
    string path = CreateDirectory(m_imagePathText->GetValue().c_str());
    m_imagePath = path.c_str();
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
    
    string path = m_imagePath.c_str();
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
            //wxMutexGuiEnter();
            int threshold = m_thresholdSlider->GetValue();
            //wxMutexGuiLeave();
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

bool MultiFabController::IsDisplayingImage()
{
    return m_acquireImage;
}

wxString MultiFabController::GetImagePath()
{
    return m_imagePath;    
}

void MultiFabController::SetFrame(MainFrame *frame)
{
    m_frame = frame;
}

void MultiFabController::Close()
{
    EmergencyStop();
    
    m_laserPtr->CloseShutter();
    m_laserPtr->SetLaserOff();
    StopGrabImage();
    ::wxUsleep(10);    
}

void MultiFabController::CheckLaserStatus()
{
    if(!IsInited()) {
        wxMessageBox("Init system first", "Error");
        return;
    }    
    
    //  wxBusyInfo info("checking laset status", this);
    string laser = m_laserPtr->GetLaserStatus();
    string shutter = m_laserPtr->GetShutterStatus();
    string powerPercent = m_laserPtr->GetPowerPercent();
    string powerWatt = m_laserPtr->GetPowerWatt();
    string epc = m_laserPtr->GetEPCStatus();
    string ilock = m_laserPtr->GetInterlockStatus();

    m_frame->SetLaserStatus(laser);
    m_frame->SetShutterStatus(shutter);
    m_frame->SetPowerPercent(powerPercent);
    m_frame->SetPowerWatt(powerWatt);
    m_frame->SetEpc(epc);
    m_frame->SetInterlock(ilock);
}

void MultiFabController::SetLaserSerialPort(int port)
{
    m_laserSerialPort = port;
}

int MultiFabController::GetLaserSerialPort()
{
    return m_laserSerialPort;
}

bool MultiFabController::OpenLaserShutter()
{
    m_laserPtr->OpenShutter();
    string shutter = m_laserPtr->GetShutterStatus();
    m_frame->SetShutterStatus(shutter.c_str());
    return true;
}

bool MultiFabController::CloseLaserShutter()
{
    m_laserPtr->CloseShutter();
    string shutter = m_laserPtr->GetShutterStatus();
    m_frame->SetShutterStatus(shutter.c_str());
    return true;
}

bool MultiFabController::DoSingleShot()
{
    m_laserPtr->SingleShot();
    return true;
}

bool MultiFabController::SetLaserPowerPercent(int percent)
{
    m_laserPtr->SetPower(percent);
    string power = m_laserPtr->GetPowerPercent();
    m_frame->SetPowerPercent(power);
    
    boost::shared_ptr<wxConfig> config(new wxConfig(Parameters::AppName));
    config->Write(Parameters::PowerPercent, percent);
    
    return true;
}

bool MultiFabController::SetLaserOn()
{
    m_laserPtr->SetLaserOn();
    string laser = m_laserPtr->GetLaserStatus();
    m_frame->SetLaserStatus(laser.c_str());
    return true;
}

bool MultiFabController::SetLaserOff()
{
    m_laserPtr->SetLaserOff();
    string laser = m_laserPtr->GetLaserStatus();
    m_frame->SetLaserStatus(laser.c_str());
    return true;
}

bool MultiFabController::SetLaserStandby()
{
    m_laserPtr->SetLaserStandby();
    string laser = m_laserPtr->GetLaserStatus();
    m_frame->SetLaserStatus(laser.c_str());
    return true;
}

void MultiFabController::ChooseImagePath()
{
    wxString dirHome;
    wxGetHomeDir(&dirHome);
    int style = wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST;

    wxDirDialog dialog(m_frame, _T("Testing directory picker"), dirHome, style);

    if (dialog.ShowModal() == wxID_OK) {
        wxString path = dialog.GetPath();
        m_frame->SetImagePath(path);

        boost::scoped_ptr<wxConfig> config(new wxConfig(Parameters::AppName));
        config->Write(Parameters::ImagePath, path);
    }    
}

bool MultiFabController::SetLaserEpcOn()
{
    m_laserPtr->SetEPCOn();
    string epc = m_laserPtr->GetEPCStatus();
    m_frame->SetEpc(epc.c_str());
    return true;
}

bool MultiFabController::Run()
{
    LaserThread *thread = new LaserThread(this);

    if(thread->Create() != wxTHREAD_NO_ERROR) {
        wxLogError("Cannot create thread");
    }

    m_frame->PrepareRun();
    thread->Run();
    return true;
}