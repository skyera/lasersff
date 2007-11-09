#include "LaserThread.h"
#include "Controller.h"

LaserThread::LaserThread(rcam::Controller *controller):wxThread(wxTHREAD_JOINABLE)
{
    m_controller = controller;
}

void* LaserThread::Entry()
{
    m_controller->MonitorLaser();
    return NULL;
}

void LaserThread::OnExit()
{
    ::wxMessageBox("Process is finsihed", "laser");
}