#include "LaserFactory.h"
#include "YagLaser.h"
#include <iostream>
#include <cassert>

using namespace rcam;
using namespace std;

boost::shared_ptr<Laser> LaserFactory::CreateLaser(const string& laserName)
{
    boost::shared_ptr<Laser> ptr;
    if(laserName == "Yag") {
        ptr = boost::shared_ptr<Laser>(new YagLaser());
    } else {
        cerr << "wrong name\n";
        
    }
    assert(ptr.get() != NULL);
    return ptr;
}