#include <tut/tut.hpp>
#include "SerialPort.h"
#include <iostream>

namespace tut
{

struct serialport_data
{

};

typedef test_group<serialport_data> factory;
typedef factory::object object;
}

namespace 
{
tut::factory tf("SerialPort");
}

namespace tut
{
template<>
template<>
void object::test<1>()
{
    // good
    rcam::SerialPort com;
    bool ok = com.Open("COM1", 9600);
    ensure(ok);
    com.Close();
}

template<>
template<>
void object::test<2>()
{
    //
    rcam::SerialPort com;
    bool ok = false;
    try {
        com.Open("abc", 9600);
    }
    catch(std::exception&) {
        ok = true;
    }
    ensure(ok);
}

template<>
template<>
void object::test<3>()
{
    rcam::SerialPort com;
    bool ok = false;
    try {
        com.Open("COM1", -1);
    }
    catch(std::exception&) {
        ok = true;
    }
    ensure(ok);
}

}