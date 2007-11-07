#include "LaserThread.h"
#include "MainFrame.h"

LaserThread::LaserThread(MainFrame *frame)
{
    m_frame = frame;
}

void* LaserThread::Entry()
{
    m_frame->MonitorLaser();
    return NULL;
}

void LaserThread::OnExit()
{
    ::wxMessageBox("Process us finsihed", "laser");
}