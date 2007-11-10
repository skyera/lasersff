#include "MainFrame.h"
#include <wx/msgdlg.h>
#include <wx/tooltip.h>
#include "ConnectDialog.h"
#include "ControllerFactory.h"
#include "LaserThread.h"
#include <niimaq.h>
#include <wx/config.h>
#include "Parameters.h"
#include <boost/scoped_ptr.hpp>
#include <wx/statline.h>
#include "DeviceCheckDialog.h"
#include <wx/log.h>
#include <wx/artprov.h>
#include <wx/busyinfo.h>
#include <wx/toolbar.h>

using namespace rcam;
using namespace std;

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_CONNECT, MainFrame::OnConnect)
    EVT_MENU(ID_DISCONNECT, MainFrame::OnDisconnect)
    EVT_MENU(ID_Quit,  MainFrame::OnQuit)
    EVT_MENU(ID_OPTIONS, MainFrame::OnOptions)
    EVT_MENU(ID_About, MainFrame::OnAbout)
    EVT_PAINT(MainFrame::OnPaint)
    EVT_BUTTON(ID_RUN, MainFrame::OnRun)
    EVT_BUTTON(ID_CHECKLASER, MainFrame::OnCheckLaser)
    EVT_BUTTON(ID_OPENSHUTTER, MainFrame::OnOpenShutter)
    EVT_BUTTON(ID_CLOSESHUTTER, MainFrame::OnCloseShutter)
    EVT_BUTTON(ID_SINGLESHOT, MainFrame::OnSingleShot)
    EVT_BUTTON(ID_ESTOP, MainFrame::OnEStop)
    EVT_BUTTON(ID_SETPOWER, MainFrame::OnSetPower)

    EVT_UPDATE_UI(ID_DISCONNECT, MainFrame::OnUpdateDisConnect)
    EVT_UPDATE_UI(ID_CONNECT, MainFrame::OnUpdateConnect)
    EVT_BUTTON(ID_LASERON, MainFrame::OnLaserOn)
    EVT_BUTTON(ID_LASEROFF, MainFrame::OnLaserOff)
    EVT_BUTTON(ID_LASERSTANDBY, MainFrame::OnLaserStandby)
    EVT_TIMER(ID_TIMER, MainFrame::OnTimer)
    EVT_CHECKBOX(ID_DISPLAY_IMAGE_CHECK, MainFrame::OnCheckDisplayImage)
    EVT_BUTTON(ID_IMAGE_PATH, MainFrame::OnChooseImagePath)
    EVT_CLOSE(MainFrame::OnClose)
    EVT_BUTTON(ID_EPC,MainFrame::OnEpc)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title):wxFrame(NULL, wxID_ANY, title)
{
    SetIcon(wxICON(sample));
    CreateMenu();
    FormToolBar();
    CreateControls();
    CreateStatusBar(2);
    SetStatusText(_T("LAser!"));
    m_timer.SetOwner(this, ID_TIMER);
    
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
    

    // controller
    wxString name("MultiFab");
    m_controller = ControllerFactory::CreateController(name);
    m_controller->SetFrame(this);

    m_controller->Init();
}

void MainFrame::FormToolBar()
{
    long style = wxTB_FLAT |wxTB_DOCKABLE|wxTB_TEXT|wxTB_TOP;//|wxTB_HORZ_LAYOUT;
    
    m_toolbar = CreateToolBar(style);
    
    wxBitmap bmp1 = wxArtProvider::GetBitmap(wxART_HARDDISK, wxART_TOOLBAR);
    wxBitmap bmp2 = wxArtProvider::GetBitmap(wxART_CDROM, wxART_TOOLBAR);
    wxBitmap bmp3 = wxArtProvider::GetBitmap(wxART_FLOPPY, wxART_TOOLBAR);
    
    GetToolBar()->SetToolBitmapSize(wxSize(bmp2.GetWidth(), bmp2.GetHeight()));
    
    m_toolbar->AddTool(ID_CONNECT, "Connect", bmp1, wxNullBitmap, wxITEM_NORMAL, "Connect", "Connect to serial port");
    m_toolbar->AddTool(ID_DISCONNECT, "Disconnect", bmp2, wxNullBitmap, wxITEM_NORMAL, "Disconnect", "Disconnect to serial port");
    m_toolbar->AddTool(ID_About, "About", bmp3, wxNullBitmap, wxITEM_NORMAL, "About", "help inforation");
    m_toolbar->Realize();

}

void MainFrame::CreateMenu()
{
    wxMenu *fileMenu = new wxMenu;
    wxMenu *helpMenu = new wxMenu;
    wxMenu *toolMenu = new wxMenu;

    fileMenu->Append(ID_CONNECT, _T("Connect"), _T("Connect to laser"));
    fileMenu->Append(ID_DISCONNECT, _T("Disconnect"), _T("Disconnect from laser"));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
    toolMenu->Append(ID_OPTIONS, _T("Options"), _T("options"));
    helpMenu->Append(ID_About, _T("&About...\tF1"), _T("Show about dialog"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
    menuBar->Append(toolMenu, _T("&Tool"));
    menuBar->Append(helpMenu, _T("&Help"));

    SetMenuBar(menuBar);
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format(
        _T("Rcam Laser Cladding Application\n")
        _T("Zhigang Liu\n")
        _T("Oct 2007")

        ),
        _T("About SFF"),
        wxOK | wxICON_INFORMATION,
        this);
}

void MainFrame::OnOptions(wxCommandEvent& event)
{
//    wxMessageDialog* dlg = new wxMessageDialog(this, _T("hi"));
//    dlg->ShowModal();
//    dlg->Destroy();
}

void MainFrame::OnConnect(wxCommandEvent& event)
{
    ConnectDialog dlg(this, "Connect");
    if(dlg.ShowModal() == wxID_OK) {
        int port = dlg.GetSerialPortNumber();
        m_controller->SetLaserSerialPort(port);
        m_controller->Init();
    }
}

void MainFrame::OnDisconnect(wxCommandEvent& event)
{
    m_controller->Disconnect();
}

void MainFrame::CreateControls()
{
    m_logoBitmap = wxBITMAP(ID_LOGO);
    m_stopBitmap = wxBITMAP(ID_BITMAP_ESTOP);
    m_runBitmap = wxBITMAP(ID_BITMAP_RUN);
    m_poolBitmap = wxBITMAP(ID_BITMAP_POOL);
    m_poniBitmap = wxBITMAP(ID_BITMAP_PONI);
    m_rcamBitmap = wxBITMAP(ID_BITMAP_RCAM);


    wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainsizer);
    wxPanel *panel = new wxPanel(this, -1);
    mainsizer->Add(panel, wxSizerFlags(1).Expand());

    wxBoxSizer *sizer_p = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(sizer_p);

    wxBoxSizer *h_sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer_p->Add(h_sizer, wxSizerFlags(1).Expand());

    wxPanel *left = CreateLeftPanel(panel);
    wxPanel *right = CreateRightPanel(panel);
    h_sizer->Add(left, wxSizerFlags(1).Expand().Border(wxALL, 5));
    h_sizer->Add(right, wxSizerFlags().Expand().Border(wxALL, 5));
}

wxPanel* MainFrame::CreateLeftPanel(wxPanel* parent)
{
    wxPanel* panel = new wxPanel(parent, -1, wxDefaultPosition, wxSize(300,200), wxTAB_TRAVERSAL);//wxBORDER_SIMPLE);//|wxBORDER_RAISED );
    wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(topsizer);

    CreateLaserStatusControls(topsizer, panel);
    CreateRunControls(panel);
    CreateLaserOperationControls(topsizer, panel);

    return panel;
}

void MainFrame::CreateLaserStatusControls(wxBoxSizer *topsizer, wxPanel* panel)
{
    //
    // status group box
    //
    wxStaticBox *statusBox = new wxStaticBox(panel, -1, "Laser Status");
    wxBoxSizer *statusBoxSizer = new wxStaticBoxSizer(statusBox, wxVERTICAL);
    topsizer->Add(statusBoxSizer, wxSizerFlags(0).Expand().Border(wxALL, 10));

    wxGridSizer *gridsizer = new wxGridSizer(5, 2, 2, 5);
    statusBoxSizer->Add(gridsizer, wxSizerFlags(1).Expand().Border(wxALL, 5));

    // laser status
    gridsizer->Add(new wxStaticText(panel, -1, "Laser"), wxSizerFlags().Align(wxALIGN_LEFT));
    m_laserStatusText = new wxTextCtrl(panel, ID_LASER_STATUS, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_laserStatusText->SetToolTip("last status: On,Off or standby");
    gridsizer->Add(m_laserStatusText, wxSizerFlags().Align(wxALIGN_LEFT).Expand());

    // shutter
    gridsizer->Add(new wxStaticText(panel, ID_SHUTTER_STATUS, "Shutter"), wxSizerFlags().Align(wxALIGN_LEFT));
    m_shutterStatusText = new wxTextCtrl(panel, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_shutterStatusText->SetToolTip("shutter: Open or close");
    gridsizer->Add(m_shutterStatusText, wxSizerFlags().Align(wxALIGN_LEFT).Expand());

    // power percent
    gridsizer->Add(new wxStaticText(panel, -1, "Power Percent(%)"), wxSizerFlags().Align(wxALIGN_LEFT));
    m_powerPercentText = new wxTextCtrl(panel, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_powerPercentText->SetToolTip("power percent (1-100)%");
    gridsizer->Add(m_powerPercentText, wxSizerFlags().Align(wxALIGN_LEFT).Expand());

    // power watt
    gridsizer->Add(new wxStaticText(panel, -1, "Power(watt)"), wxSizerFlags().Align(wxALIGN_LEFT));
    m_powerWattText = new wxTextCtrl(panel, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_powerWattText->SetToolTip("power in watt");
    gridsizer->Add(m_powerWattText, wxSizerFlags().Align(wxALIGN_LEFT).Expand());

    // EPC
    gridsizer->Add(new wxStaticText(panel, -1, "EPC"), wxSizerFlags().Align(wxALIGN_LEFT));
    m_epcText = new wxTextCtrl(panel, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    gridsizer->Add(m_epcText, wxSizerFlags().Align(wxALIGN_LEFT).Expand());

    // Interlock
    gridsizer->Add(new wxStaticText(panel, -1, "Interlock"), wxSizerFlags().Align(wxALIGN_LEFT));
    m_interlockText = new wxTextCtrl(panel, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    gridsizer->Add(m_interlockText, wxSizerFlags().Align(wxALIGN_LEFT).Expand());
}

void MainFrame::CreateLaserOperationControls(wxBoxSizer *topsizer, wxPanel *panel)
{
    //
    // Operation group box
    //
    wxStaticBox *operationBox = new wxStaticBox(panel, -1, "Operation");

    wxBoxSizer *operationBoxSizer = new wxStaticBoxSizer(operationBox, wxVERTICAL);
    topsizer->Add(operationBoxSizer, wxSizerFlags(1).Expand().Border(wxALL, 10));

    // set laser power
    wxBoxSizer *setPowerSizer = new wxBoxSizer(wxHORIZONTAL);
    operationBoxSizer->Add(setPowerSizer, wxSizerFlags(1).Align(wxALIGN_LEFT).Expand());
    setPowerSizer->Add(new wxStaticText(panel, -1, "Laser Power"), 
        wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL, 5));
    m_powerSpinCtrl = new wxSpinCtrl(panel, -1, "51");
    setPowerSizer->Add(m_powerSpinCtrl, wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL, 5)); 

    {
        wxBoxSizer *s = new wxBoxSizer(wxHORIZONTAL);
        operationBoxSizer->Add(s, wxSizerFlags(1).Expand());
        m_setPowerButton = new wxButton(panel, ID_SETPOWER, "Set Power");
        s->Add(m_setPowerButton, wxSizerFlags(1).Align(wxALIGN_LEFT).Border(wxALL, 5).Expand());

        //s->AddStretchSpacer(1);

        m_epcButton = new wxButton(panel, ID_EPC, "Set EPC");
        s->Add(m_epcButton, wxSizerFlags(1).Left().Border(wxALL, 5).Expand());
        // check laser
        m_checkLaserButton = new wxButton(panel, ID_CHECKLASER, "Check Laser");
        //m_checkLaserButton->SetToolTip("retrieve laset status information");
        s->Add(m_checkLaserButton, wxSizerFlags(1).Align(wxALIGN_LEFT).Border(wxALL, 5).Expand());
    }

    // laser on/standby
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    operationBoxSizer->Add(sizer, wxSizerFlags(1).Align(wxALIGN_LEFT).Expand());

    m_laserOnButton = new wxButton(panel, ID_LASERON, "Laser On");
    sizer->Add(m_laserOnButton, wxSizerFlags(1).Align(wxALIGN_LEFT).Border(wxALL, 5).Expand());

    m_laserOffButton = new wxButton(panel, ID_LASEROFF, "Laser Off");
    sizer->Add(m_laserOffButton, wxSizerFlags(1).Align(wxALIGN_LEFT).Border(wxALL, 5).Expand());

    m_laserStandbyButton = new wxButton(panel, ID_LASERSTANDBY, "Laser Standby");
    sizer->Add(m_laserStandbyButton, wxSizerFlags(1).Align(wxALIGN_LEFT).Border(wxALL, 5).Expand());

    // shutter
    {
        wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
        operationBoxSizer->Add(sizer, wxSizerFlags(1).Align(wxALIGN_LEFT).Expand());

        m_openShutterButton = new wxButton(panel, ID_OPENSHUTTER, "Open Shutter");
        sizer->Add(m_openShutterButton, wxSizerFlags(1).Align(wxALIGN_LEFT).Border(wxALL, 5).Expand());

        m_closeShutterButton = new wxButton(panel, ID_CLOSESHUTTER, "Close Shutter");
        sizer->Add(m_closeShutterButton, wxSizerFlags(1).Align(wxALIGN_LEFT).Border(wxALL, 5).Expand());

        m_singleShotButton = new wxButton(panel, ID_SINGLESHOT, "single shot");
        sizer->Add(m_singleShotButton, wxSizerFlags(1).Align(wxALIGN_CENTER).Border(wxALL, 5).Expand());
    }
}

void MainFrame::CreateRunControls(wxPanel *parent)
{
    wxPanel *p = new wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE);//wxBORDER_STATIC);//wxBORDER_SIMPLE);//wxBORDER_RAISED );

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    p->SetSizer(sizer);
    wxSizer *topsizer = parent->GetSizer();
    topsizer->Add(p, wxSizerFlags().Center().Expand().Border(wxALL, 5));

    sizer->AddStretchSpacer(1);
    // stop
    m_estopButton = new wxBitmapButton(p, ID_ESTOP, m_stopBitmap);
    //m_estopButton->SetToolTip("Emergency stop: turn off laser");
    sizer->Add(m_estopButton, wxSizerFlags().Border(wxALL, 10));

    sizer->AddStretchSpacer(1);
    // run
    m_runButton = new wxBitmapButton(p, ID_RUN, m_runBitmap);
    //m_runButton->SetToolTip("start laser 3D cladding");
    sizer->Add(m_runButton, wxSizerFlags().Border(wxALL, 10));
    sizer->AddStretchSpacer(1);

    {
        wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
        parent->GetSizer()->Add(box, wxSizerFlags().Expand().Border(wxALL, 5).Center());

        box->Add(new wxStaticText(parent, -1, "Time(secs)"), 
            wxSizerFlags().Border(wxALL, 5).Left());
        m_processTimeText = new wxTextCtrl(parent, -1, "0");

        box->Add(m_processTimeText, wxSizerFlags(1).Border(wxALL, 5).Left());
        box->AddStretchSpacer(1);
        box->Add(new wxStaticText(parent, -1, "Status"), 
            wxSizerFlags().Border(wxALL, 5).Right());
        m_processStatusText = new wxTextCtrl(parent, ID_PROCESS_STATUS, "Not started");
        box->Add(m_processStatusText, wxSizerFlags(1).Expand().Border(wxALL, 5).Left());
    }
}

void MainFrame::OnRun(wxCommandEvent& run)
{
    DeviceCheckDialog dlg(this, "Check Devices");
    int ok = dlg.ShowModal();

    if(ok == wxID_OK && dlg.IsRun()) {
        m_controller->Run();
    }
}

void MainFrame::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    
    if(!m_controller->IsDisplayingImage()) {
        wxClientDC dc(m_poolPanel);
        dc.DrawBitmap(m_poolBitmap, 0, 0);
    }

    // rcam logo
    {
        wxClientDC dc(m_rcamPanel);
        dc.DrawBitmap(m_rcamBitmap, 0, 0);
    }
}

wxPanel* MainFrame::CreateRightPanel(wxPanel *parent)
{
    wxPanel* panel = new wxPanel(parent, -1, wxDefaultPosition, wxSize(200,200), wxTAB_TRAVERSAL);//|wxBORDER_SIMPLE);//|wxBORDER_RAISED );
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(topsizer);

    {
        wxStaticBoxSizer *box = new wxStaticBoxSizer(wxVERTICAL, panel, "Molten pool image");
        topsizer->Add(box, wxSizerFlags().Expand());

        wxPanel *uppanel = new wxPanel(panel, -1);
        box->Add(uppanel, wxSizerFlags().Center().Border(wxALL, 5));

        wxBoxSizer *upsizer = new wxBoxSizer(wxVERTICAL);
        uppanel->SetSizer(upsizer);

        m_poolPanel = new wxPanel(uppanel, 0, 0, 
            m_poolBitmap.GetWidth(),
            m_poolBitmap.GetHeight(), 
            wxBORDER_SUNKEN);
        upsizer->Add(m_poolPanel, wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL, 5));

        m_displayImageCheck = new wxCheckBox(uppanel, ID_DISPLAY_IMAGE_CHECK, "Display Image");
        upsizer->Add(m_displayImageCheck, wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL, 5).Expand());

        m_binaryImageCheck = new wxCheckBox(uppanel, -1, "Binary Image");
        upsizer->Add(m_binaryImageCheck, wxSizerFlags().Left().Border(wxALL, 5).Expand());

        {

            upsizer->Add(new wxStaticText(uppanel, -1, "Threshold"));
            m_thresholdSlider = new wxSlider(uppanel, -1, 70, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS);
            upsizer->Add(m_thresholdSlider, wxSizerFlags(0).Expand());
        }

        // save option
        wxArrayString str;
        str.Add("Auto save");
        str.Add("Manual save");

        m_saveOptionRadioBox = new wxRadioBox(uppanel, -1, "Save Option", 
                                              wxDefaultPosition, wxDefaultSize, str,
                                              2, wxRA_SPECIFY_COLS);
        upsizer->Add(m_saveOptionRadioBox, wxSizerFlags().Border(wxALL, 5).Expand());

        m_saveImageCheck = new wxCheckBox(uppanel, -1, "Save Image");
        upsizer->Add(m_saveImageCheck, wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL, 5).Expand());

//        upsizer->Add(new wxStaticText(uppanel, -1, "Maximum number of images to save"), wxSizerFlags().Align(wxALIGN_LEFT));
//        m_maxNumImageText = new wxTextCtrl(uppanel, -1, "2000");
//        upsizer->Add(m_maxNumImageText, wxSizerFlags().Align(wxALIGN_LEFT).Expand());

        upsizer->Add(new wxStaticText(uppanel, -1, "Number of images saved"), wxSizerFlags().Align(wxALIGN_LEFT));
        m_numSavedImageText = new wxTextCtrl(uppanel, -1, "0");
        upsizer->Add(m_numSavedImageText, wxSizerFlags().Align(wxALIGN_LEFT));

        upsizer->Add(new wxStaticText(uppanel, -1, "Image path"), wxSizerFlags().Left());

        m_imagePathText = new wxTextCtrl(uppanel, -1, "c:\\images");

        wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
        upsizer->Add(hbox, wxSizerFlags().Expand());
        hbox->Add(m_imagePathText, wxSizerFlags(1).Left().Expand());
        wxButton *o = new wxButton(uppanel, ID_IMAGE_PATH, "Browse");
        hbox->Add(o, wxSizerFlags());
    }

    topsizer->AddStretchSpacer(1);
    wxStaticText *rcam = new wxStaticText(panel, -1, "SMU RCAM Laser Center");
    rcam->SetForegroundColour(*wxBLUE);
    wxFont font(12, wxFONTFAMILY_ROMAN, wxITALIC, wxBOLD, false);
    rcam->SetFont(font);
    topsizer->Add(rcam, wxSizerFlags().Center().Border(wxALL, 5));
    m_rcamPanel = new wxPanel(panel, -1, wxDefaultPosition, wxSize(m_rcamBitmap.GetWidth(), m_rcamBitmap.GetHeight()), wxBORDER_SUNKEN );

    topsizer->Add(m_rcamPanel, wxSizerFlags().Center().Border(wxALL, 5));

    return panel;
}

void MainFrame::FinishRun()
{
    m_timer.Stop();
    m_runButton->Enable();

    m_processStatusText->SetLabel("Finished");
}

void MainFrame::OnCheckLaser(wxCommandEvent& event)
{
    m_controller->CheckLaserStatus();
}

void MainFrame::OnOpenShutter(wxCommandEvent &event)
{
    m_controller->OpenLaserShutter();
}

void MainFrame::OnCloseShutter(wxCommandEvent &event)
{
    m_controller->CloseLaserShutter();
}

void MainFrame::OnSingleShot(wxCommandEvent &event)
{
    m_controller->DoSingleShot();
}

void MainFrame::OnEStop(wxCommandEvent& event)
{
    m_controller->Close();
}

void MainFrame::UpdateUI(const wxString& info, bool enable)
{
    SetStatusText(info);

    m_checkLaserButton->Enable(enable);
    m_openShutterButton->Enable(enable);
    m_closeShutterButton->Enable(enable);
    m_singleShotButton->Enable(enable);
    m_estopButton->Enable(enable);
    m_runButton->Enable(enable);
    m_setPowerButton->Enable(enable);
    m_laserOnButton->Enable(enable);
    m_laserOffButton->Enable(enable);
    m_laserStandbyButton->Enable(enable);
    m_epcButton->Enable(enable);
}

void MainFrame::OnUpdateDisConnect(wxUpdateUIEvent& event)
{
    event.Enable(m_controller->IsInited());
}

void MainFrame::OnUpdateConnect(wxUpdateUIEvent& event)
{
    event.Enable(!m_controller->IsInited());
}

void MainFrame::OnSetPower(wxCommandEvent& event)
{
    int power = m_powerSpinCtrl->GetValue();        
    if(power > 0 && power <= 100) {
        m_controller->SetLaserPowerPercent(power);
    } else {
        ::wxMessageBox("Power range (1-100)", "error");
    }
}

void MainFrame::OnLaserOn(wxCommandEvent& event)
{
    m_controller->SetLaserOn();
}

void MainFrame::OnLaserOff(wxCommandEvent& event)
{
    m_controller->SetLaserOff();
}

void MainFrame::OnLaserStandby(wxCommandEvent& event)
{
    m_controller->SetLaserStandby();
}

void MainFrame::OnTimer(wxTimerEvent &event)
{
    wxString str;
    m_processTime += event.GetInterval()/1000;
    str << m_processTime;
    m_processTimeText->SetLabel(str);        
}

void MainFrame::OnClose(wxCloseEvent &event)
{
    event.SetCanVeto(true);
    //  event.Veto(false);
    int ok = ::wxMessageBox("Do you want to exit?", "Exit", wxYES_NO);   
    if(ok == wxYES) {
        Hide();

        m_controller->Close();
        Destroy();
    } else {
        event.Veto();
    }
}

void MainFrame::OnCheckDisplayImage(wxCommandEvent& event)
{
    if(m_displayImageCheck->GetValue()) {
        wxBusyInfo info("setup frame grabber, please wait", this);
        bool ok = m_controller->StartGrabImage();

        if(!ok) {
            m_displayImageCheck->SetValue(false);
        } else {
            wxString str;
            str << "Images are saved in "<< m_controller->GetImagePath();
            SetStatusText(str, 1);
        }
    } else {
        m_controller->StopGrabImage();
        Update();
    }        
}

void MainFrame::OnChooseImagePath(wxCommandEvent& event)
{
    m_controller->ChooseImagePath();
}

void MainFrame::OnEpc(wxCommandEvent &event)
{
    m_controller->SetLaserEpcOn();
}

void MainFrame::SetLaserStatus(const wxString& laser)
{
    m_laserStatusText->SetLabel(laser);
    if(laser == "On") {
        m_laserStatusText->SetBackgroundColour(*wxRED);
    } else {
        m_laserStatusText->SetBackgroundColour(*wxWHITE);
    }
}

void MainFrame::SetShutterStatus(const wxString& shutter)
{
    m_shutterStatusText->SetLabel(shutter);
    if(shutter == "Open") {
        m_shutterStatusText->SetBackgroundColour(*wxRED);
    } else {
        m_shutterStatusText->SetBackgroundColour(*wxWHITE);
    }
 
}

void MainFrame::SetPowerPercent(const wxString& powerPercent)
{
    m_powerPercentText->SetLabel(powerPercent);
}

void MainFrame::SetPowerWatt(const wxString& powerWatt)
{
    m_powerWattText->SetLabel(powerWatt);
}

void MainFrame::SetEpc(const wxString& epc)
{
    m_epcText->SetLabel(epc);
}

void MainFrame::SetInterlock(const wxString& lock)
{
    m_interlockText->SetLabel(lock);
}

void MainFrame::SetImagePath(const wxString& path)
{
    m_imagePathText->SetLabel(path);
}

void MainFrame::SetPowerSpin(int power)
{
    m_powerSpinCtrl->SetValue(power);
}

void MainFrame::PrepareRun()
{
    m_runButton->Enable(false);
    m_timer.Start(1000);
    m_processTime = 0;
    m_processStatusText->SetLabel("Running....");    
}


HWND MainFrame::GetDisplayWindow()
{
    return static_cast<HWND>(m_poolPanel->GetHandle());
}

int MainFrame::GetMaxNumSaveImages()
{
    long max = 0;
    m_maxNumImageText->GetValue().ToLong(&max);
    return max;
}

bool MainFrame::IsSaveImage()
{
    return m_saveImageCheck->GetValue();
}

void MainFrame::SetSavedImageNumber(int num)
{
    wxString s;
    s << num;
    m_numSavedImageText->SetLabel(s);
}

bool MainFrame::IsDisplayBinaryImage()
{
    return m_binaryImageCheck->GetValue();
}

int MainFrame::GetThreshold()
{
    return m_thresholdSlider->GetValue();
}

wxString MainFrame::GetImagePath()
{
    return m_imagePathText->GetValue();
}

bool MainFrame::IsAutoSaveImage()
{
    return m_saveOptionRadioBox->GetSelection() == 0;
}