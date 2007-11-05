#include "LaserApp.h"
#include "MainFrame.h"

bool LaserApp::OnInit()
{
    if(!wxApp::OnInit()) {
        return false;
    }
    
    MainFrame* frame = new MainFrame(_T("RCAM Laser Application"));
    frame->Show(true);
    return true;    
}