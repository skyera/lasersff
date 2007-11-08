#include "NiFrameGrabber.h"
#include <wx/wx.h>

using namespace rcam;
using namespace std;

NiFrameGrabber::NiFrameGrabber():m_interfaceName("img0"), m_Iid(0), m_Sid(0)
{
    m_opened = false;
}

NiFrameGrabber::~NiFrameGrabber()
{
    Close();
}

bool NiFrameGrabber::AcquireImage()
{
    m_ImaqBuffer = NULL;
    int st = imgGrab(m_Sid, reinterpret_cast<void**>(&m_ImaqBuffer), TRUE);
    if(st < 0) {
        return false;
    } else {
        return true;
    }
}

void NiFrameGrabber::DisplayImage(HWND hwnd)
{
    imgPlot((GUIHNDL)hwnd, m_ImaqBuffer, 
        0, 0, m_AcqWinWidth, m_AcqWinHeight,
        0, 0, m_plotFlag);
}

void NiFrameGrabber::SaveImage(const string& filename)
{
    imgSessionSaveBufferEx(m_Sid, m_ImaqBuffer, const_cast<char*>(filename.c_str()));
}

bool NiFrameGrabber::Init()
{
    m_opened = false;
    int st = imgInterfaceOpen(const_cast<Int8*>(m_interfaceName.c_str()), &m_Iid);
    if(st < 0) {
        DisplayImgError(st);
        return false;
    }

    st = imgSessionOpen(m_Iid, &m_Sid);
    if(st < 0) {
        return false;
    }

    st = imgGetAttribute (m_Sid, IMG_ATTR_ROI_WIDTH, &m_AcqWinWidth);
    if(st < 0) {
        return false;
    }

    st = imgGetAttribute (m_Sid, IMG_ATTR_ROI_HEIGHT, &m_AcqWinHeight);
    if(st < 0) {
        return false;
    }

    st = imgGetAttribute (m_Sid, IMG_ATTR_BITSPERPIXEL, &m_bitsPerPixel);
    if(st < 0) {
        return false;
    }
    switch(m_bitsPerPixel) {
    case 10:
        m_plotFlag = IMGPLOT_MONO_10;
        break;
    case 12:
        m_plotFlag = IMGPLOT_MONO_12;
        break;
    case 14:
        m_plotFlag = IMGPLOT_MONO_14;
        break;
    case 16:
        m_plotFlag = IMGPLOT_MONO_16;
        break;
    case 24:
    case 32:
        // assumes that a 24 bits camera is a color camera.
        // in this mode, even if the camera is 24 bits the board returns 32 bits values
        m_plotFlag = IMGPLOT_COLOR_RGB32;
        break;
    default:
        m_plotFlag = IMGPLOT_MONO_8;
        break;
    }

    st = imgGrabSetup(m_Sid, TRUE);
    if(st < 0) {
        DisplayImgError(st);
        return false;
    }
    m_opened = true;
    return true;
}


bool NiFrameGrabber::Close()
{
    if(!m_opened) {
        return true;
    }
    int st = imgSessionStopAcquisition(m_Sid);
    if(st < 0) {
        return false;
    }

    imgClose (m_Sid, TRUE);
    imgClose (m_Iid, TRUE);
    m_Sid = 0;
    m_Iid = 0;
    m_opened = false;
    return true;
}

void NiFrameGrabber::DisplayImgError(Int32 error)
{
    static Int8 ErrorMessage[256];

    memset(ErrorMessage, 0x00, sizeof(ErrorMessage));

    // converts error code to a message
    imgShowError(error, ErrorMessage);
    wxMessageBox(ErrorMessage, "Error");
}

void NiFrameGrabber::BinaryImage(int threshold)
{
    for(int i = 0; i < m_AcqWinHeight; i++) {
        for(int j = 0; j < m_AcqWinWidth; j++) {
            int pos = i * m_AcqWinWidth + j;
            int value = m_ImaqBuffer[pos];
            if(value > threshold) {
                m_ImaqBuffer[pos] = 255;
            } else {
                m_ImaqBuffer[pos] = 0;
            }
        }
    }    
}