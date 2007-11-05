#ifndef RCAM_LASERFACTORY_H
#define RCAM_LASERFACTORY_H

#include "Laser.h"
#include <string>
#include <boost/shared_ptr.hpp>

namespace rcam
{
class LaserFactory
{
public:
    static boost::shared_ptr<Laser> CreateLaser(const std::string& laserName);
private:
    LaserFactory();
};

}

#endif