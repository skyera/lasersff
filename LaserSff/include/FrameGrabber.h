#ifndef RCAM_FRAME_GRABBER_H
#define RCAM_FRAME_GRABBER_H

#include <string>
#include <wx/wx.h>
namespace rcam {

class FrameGrabber
{
public:
    FrameGrabber(){}
    virtual ~FrameGrabber(){};

    virtual bool AcquireImage() = 0;
    virtual void DisplayImage(HWND hwnd) = 0;
    virtual void SaveImage(const std::string& filename) = 0;
    virtual void BinaryImage(int threshold) = 0;
    virtual bool Init() = 0;
    virtual bool Close() = 0;
};

}

#endif