#include "YagLaser.h"
#include <sstream>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace rcam;
using namespace std;


YagLaser::YagLaser(): m_connected(false)
{

}

YagLaser::~YagLaser()
{

}

bool YagLaser::Connect(int port)
{
    if(m_connected) {
        return true;
    }

    m_serialPortPtr = boost::shared_ptr<wxSerialPort>(new wxSerialPort());
    stringstream ss;
    ss << "com" << port;

    int ok = m_serialPortPtr->Open(ss.str().c_str());
    if(ok < 0) {
        return false;
    }
    m_serialPortPtr->SetBaudRate(wxBAUD_4800);
    m_connected = true;
    return true;
}

bool YagLaser::Disconnect()
{
    if(m_serialPortPtr.get()) {
        m_serialPortPtr->Close();
        m_connected = false;
    }
    return true;
}

bool YagLaser::OpenShutter()
{
    string cmd("C1\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::CloseShutter()
{
    string cmd("C0\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SetLaserOn()
{
    string cmd("B2\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SetLaserOff()
{
    string cmd("B0\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SetLaserStandby()
{
    string cmd("B1\r");

    Write(cmd);
    ReadResponse();
    return true;
}

string YagLaser::LaserStatus()
{
    if(!m_connected) {
        return "not connected";
    }

    string cmd("O1\r");
    
    Write(cmd);
    string st = ReadResponse();
    if(st == "OF") {
        
    } else if(st == "ST") {
        
    } else if(st == "ON") {
    
    } else if(st == "S1") {
    
    } else if(st == "S2") {
    
    } else {
        st = "Unknown";
    }

    return st;
}

string YagLaser::ShutterStatus()
{
    string cmd("O2\r");
    
    Write(cmd);
    string st = ReadResponse();
    if(st != "OP" && st != "CL") {
        return "Unknown";
    }
    return st;
}

string YagLaser::PowerPercent()
{
    string cmd("OA\r");
    Write(cmd);
    string response = ReadResponse();

    try {
        boost::lexical_cast<int>(response);
    } catch(boost::bad_lexical_cast &err) {
        response = "unknown";
    }
    return response;
}

string YagLaser::PowerWatt()
{
    string cmd("O3\r");
    Write(cmd);
    string response = ReadResponse();
    try {
        boost::lexical_cast<int>(response);
    } catch(boost::bad_lexical_cast& err) {
        response = "unknown";
    }
    return response;
}

bool YagLaser::SetEPCOn()
{
    if(!m_connected) {
        return false;
    }
    
    string cmd("i1\r");

    Write(cmd);
    ReadResponse();
    SetEPCHighValue();
    SetEPCLowValue();
    return true;
}

bool YagLaser::SetEPCOff()
{
    string cmd("i0\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SingleShot()
{
    string cmd("X\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SetPower(int percent)
{
//    stringstream ss;
//    ss << "G" << percent;

//    Write(ss.str());
//    ReadResponse();
    m_daqboard->SetLaserPower(percent);
    return true;
}

bool YagLaser::SetEPCHighValue()
{
    string cmd("g100\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SetEPCLowValue()
{
    string cmd("h0\r");
    Write(cmd);
    ReadResponse();
    return true;
}

string YagLaser::EPCStatus()
{
    string cmd("0j\r");

    Write(cmd);
    string response = ReadResponse();

    try {
        boost::lexical_cast<int>(response);
    } catch(boost::bad_lexical_cast& err) {
        response = "unknown";
    }
    return response;
}

string YagLaser::ReadResponse()
{
    char *buf = NULL;
    m_serialPortPtr->ReadUntilEOS(&buf);

    string response = buf;
    delete [] buf;
    return response;
}

void YagLaser::Write(const string& cmd)
{
    m_serialPortPtr->Write(const_cast<char*>(cmd.c_str()), cmd.size());
}

string YagLaser::InterlockStatus()
{
    string cmd("O5\r");
    Write(cmd);
    string response = ReadResponse();
    
    static char *Interlock[] = {"OK", // 0
						"Interlock Diagnostic Failure",//1
						"PSU 1 Thermal Overload", //2
						"PSU 2 Thermal Overload (Not MW500)", //3
		                "PSU 3 Thermal Overload (MW2000 only)", //4
						"PSU 4 Thermal Overload (MW2000 only)",//5
		                "DC 1 Thermal Overload", //6
		                "DC 2 Thermal Overload (Not MW500)",//7
		                "DC 3 Thermal Overload (MW2000 only)", //8
		                "Cooling Water Level Low", //9
		                "Keyswitch Off",//10
		                "Shutter Failure",//11
		                "Shutter Temperature High",//12
		                "Fibre Continuity Monitor 1",//13
		                "Cooling Water Temperature High",//14
		                "Beam Dump Water Detector",//15
		                "Fibre Continuity Monitor 2 (if fitted)",//16
		                "- Unused",//17
		                "Customer remote interlock",//18
		                "Pump Overload",//19
		                "Pumping Chamber 1 Water Flow low", //20
		                "P/Chamber 2 Water Flow low (Not MW500)", //21
		                "P/Chamber 3 Water Flow low (MW2000 only)",//22
		                "P/Chamber 4 Water Flow low (MW2000 only)",//23
		                "Beam Dump Water Flow Low",//24
		                "Interlock Not Determined",//25
		                "Power Trip (Hi) Activated",//26
		                "Power Trip (Lo) Activated",//27
		                "Lamp Life Trip Activated",//28
		                "DC Bus Voltage Low or Remote Emergency-stop"
    };//29
    
    int index = -1;
    try {
        index = boost::lexical_cast<int>(response);
    } catch(boost::bad_lexical_cast& err) {
        return "unknown";
    }

    int n = sizeof(Interlock);
    if(index >= 0 && index < n) {
        return Interlock[index];
    } else {
        return "Unknown";
    }
}

void YagLaser::SetDaqboard(const boost::shared_ptr<DaqBoard>& board)
{
    m_daqboard = board;
}
