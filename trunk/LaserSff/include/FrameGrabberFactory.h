#ifndef RCAM_FRAMEGRABBER_FACTORY_H
#define RCAM_FRAMEGRABBER_FACTORY_H

#include <boost/shared_ptr.hpp>
#include "FrameGrabber.h"
#include <string>

namespace rcam {

class FrameGrabberFactory
{
public:
    static boost::shared_ptr<FrameGrabber> CreateFrameGrabber(const std::string &name);
};

}

#endif