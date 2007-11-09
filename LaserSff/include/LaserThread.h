#ifndef LASER_THREAD_H
#define LASER_THREAD_H

#include <wx/thread.h>

namespace rcam {
    class Controller;
}

class LaserThread: public wxThread
{
public:
    LaserThread(rcam::Controller *controller);

    virtual void* Entry();
    virtual void OnExit();
private:
    rcam::Controller *m_controller;
};

#endif