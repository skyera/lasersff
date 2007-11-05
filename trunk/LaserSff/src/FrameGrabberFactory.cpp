#include "FrameGrabberFactory.h"

#include "NiFrameGrabber.h"
using namespace rcam;

boost::shared_ptr<FrameGrabber> FrameGrabberFactory::CreateFrameGrabber(const std::string &name)
{
    boost::shared_ptr<FrameGrabber> p;
    if(name == "ni1402") {
        p  = boost::shared_ptr<FrameGrabber>(new NiFrameGrabber());
    }

    assert(p.get());
    return p;
}