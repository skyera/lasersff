#ifndef LASER_THREAD_H
#define LASER_THREAD_H

#include <wx/thread.h>

class MainFrame;

class LaserThread: public wxThread
{
public:
    LaserThread(MainFrame *frame);

    virtual void* Entry();
    virtual void OnExit();
private:
    MainFrame *m_frame;
};

#endif