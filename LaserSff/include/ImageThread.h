#ifndef IMAGE_THREAD_H
#define IMAGE_THREAD_H

#include <wx/thread.h>
#include "MultiFabController.h"

class ImageThread: public wxThread
{
public:
    ImageThread(rcam::MultiFabController *controller);

    virtual void* Entry();
    virtual void OnExit();

private:
    rcam::MultiFabController *m_controller;
};

#endif