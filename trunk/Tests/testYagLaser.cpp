#include <tut/tut.hpp>
#include "YagLaser.h"

namespace tut
{

struct yaglaser_data
{

};

typedef test_group<yaglaser_data> tf;
typedef tf::object object;

}

namespace 
{

tut::tf yaglaser_group("YagLaser");

}

namespace tut
{
template<>
template<>
void object::test<1>()
{
    // connect, disconnect
    rcam::YagLaser laser;
    bool ok = laser.Connect(1);
    ensure(ok);
    
    ok = laser.Disconnect();
    ensure(ok);
}

}