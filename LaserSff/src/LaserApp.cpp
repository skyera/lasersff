#include "LaserApp.h"
#include "MainFrame.h"

bool LaserApp::OnInit()
{
    if(!wxApp::OnInit()) {
        return false;
    }

    const wxString name = wxString::Format("LaserApp-%s", wxGetUserId().c_str());
    m_checker = boost::shared_ptr<wxSingleInstanceChecker>(new wxSingleInstanceChecker(name));
    if(m_checker->IsAnotherRunning()) {
        wxLogError("Program already running, aborting.");
        return false;
    }
    
    MainFrame* frame = new MainFrame(_T("RCAM Laser Application"));
    frame->Show(true);
    return true;    
}