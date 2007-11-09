#include "ImageThread.h"

ImageThread::ImageThread(rcam::MultiFabController *controller)
    :wxThread(wxTHREAD_JOINABLE), m_controller(controller)

{

}

void* ImageThread::Entry()
{
    m_controller->DoAcquireImage();
    return NULL;
}

void ImageThread::OnExit()
{

}