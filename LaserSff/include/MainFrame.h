#ifndef RCAM_MAINFRAME_H
#define RCAM_MAINFRAME_H

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/spinctrl.h>
#include "Controller.h"
#include "Laser.h"
#include <wx/listbox.h>
#include <wx/event.h>
#include <wx/timer.h>

class MainFrame: public wxFrame
{
public:
    MainFrame(const wxString& title);
    
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnOptions(wxCommandEvent& event);
    void OnConnect(wxCommandEvent& event);
    void OnDisconnect(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnRun(wxCommandEvent& event);
    void MonitorLaser();
    void OnCheckLaser(wxCommandEvent& event);
    void OnOpenShutter(wxCommandEvent& event);
    void OnCloseShutter(wxCommandEvent& event);
    void OnSingleShot(wxCommandEvent& event);
    void OnEStop(wxCommandEvent& event);
    void OnUpdateDisConnect(wxUpdateUIEvent &event);
    void OnUpdateConnect(wxUpdateUIEvent &event);
    void OnSetPower(wxCommandEvent& event);
    void OnLaserOn(wxCommandEvent& event);
    void OnLaserOff(wxCommandEvent& evnet);
    void OnLaserStandby(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnCheckDisplayImage(wxCommandEvent& event);
    void OnChooseImagePath(wxCommandEvent& event);
    void OnEpc(wxCommandEvent& event);
private:
    void CreateMenu();
    void CreateControls();
    wxPanel* CreateLeftPanel(wxPanel* parent);
    wxPanel* CreateRightPanel(wxPanel* panel);
    void FormToolBar();
    void UpdateUI(bool enable);
    void ConnectToCom();
    
    void CreateLaserStatusControls(wxBoxSizer *topsizer, wxPanel *panel);
    void CreateLaserOperationControls(wxBoxSizer *topsizer, wxPanel *panel);
    void CreateRunControls(wxPanel *parent);

    // data
    boost::shared_ptr<rcam::Controller> m_controller;
    boost::shared_ptr<rcam::Laser> m_laser;

    wxPanel *m_logoPanel;
    wxBitmap m_logoBitmap;
    wxBitmap m_stopBitmap;
    wxBitmap m_runBitmap;
    wxBitmap m_rcamBitmap;
    wxPanel *m_rcamPanel;
    wxToolBar *m_toolbar;
    
    wxBitmapButton *m_runButton;
    wxButton *m_checkLaserButton;
    wxButton *m_openShutterButton;
    wxButton *m_closeShutterButton;
    wxButton *m_singleShotButton;
    wxButton *m_setPowerButton;
    wxBitmapButton *m_estopButton;
    wxSpinCtrl *m_powerSpinCtrl;
    wxPanel *m_poolPanel;
    wxBitmap m_poolBitmap;
    wxBitmap m_poniBitmap;

    wxButton *m_laserOnButton;
    wxButton *m_laserOffButton;
    wxButton *m_laserStandbyButton;

    wxTextCtrl *m_laserStatusText;
    wxTextCtrl *m_shutterStatusText;
    wxTextCtrl *m_powerPercentText;
    wxTextCtrl *m_powerWattText;
    wxTextCtrl *m_epcText;
    wxTextCtrl *m_interlockText;
    wxTextCtrl *m_processTimeText;

    wxCheckBox *m_displayImageCheck;
    wxCheckBox *m_binaryImageCheck;
    wxCheckBox *m_saveImageCheck;
    wxTextCtrl *m_imagePathText;
    wxTextCtrl *m_processStatusText;
    wxSlider *m_thresholdSlider;
    wxTextCtrl *m_maxNumImageText;
    wxTextCtrl *m_numSavedImageText;
    wxButton *m_epcButton;

    wxTimer m_timer;
    int m_processTime;
    enum
    {
        // menu items
        ID_CONNECT = 100,
        ID_DISCONNECT,
        ID_Quit,
        ID_RUN,
        ID_OPTIONS, 
        ID_CHECKLASER,
        ID_OPENSHUTTER,
        ID_CLOSESHUTTER,
        ID_SINGLESHOT,
        ID_ESTOP,
        ID_SETPOWER,
        ID_LASERON,
        ID_LASEROFF,
        ID_LASERSTANDBY,
        ID_TIMER,
        ID_DISPLAY_IMAGE_CHECK,
        ID_IMAGE_PATH,
        ID_PROCESS_STATUS,
        ID_EPC,
        ID_About = wxID_ABOUT
    };
    DECLARE_EVENT_TABLE()
};

#endif