#ifndef RCAM_NI_FRAME_GRABBER_H
#define RCAM_NI_FRAME_GRABBER_H

#include "FrameGrabber.h"
#include <niimaq.h>
#include <string>

namespace rcam {

class NiFrameGrabber: public FrameGrabber
{
public:
    NiFrameGrabber();
    ~NiFrameGrabber();

    virtual bool AcquireImage();
    virtual void DisplayImage(HWND hwnd);
    virtual void SaveImage(const std::string& filename);
    virtual bool Init();
    virtual bool Close();
    virtual void BinaryImage(int threshold);
    
private:
    void DisplayImgError(Int32 error);


    std::string m_interfaceName;
    INTERFACE_ID m_Iid;
    SESSION_ID   m_Sid;
    Int32 m_AcqWinWidth;
    Int32 m_AcqWinHeight;
	unsigned int m_bitsPerPixel;
    unsigned int m_plotFlag;
    Int8 *m_ImaqBuffer;

};

}

#endif