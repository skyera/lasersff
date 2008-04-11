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
tut::factory tf("serial port operation");
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
    
    //
    try {
        com.Open("abc", 9600);
    }
    catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
}

}