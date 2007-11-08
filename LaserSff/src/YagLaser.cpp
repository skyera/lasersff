#include "YagLaser.h"
#include <sstream>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace rcam;
using namespace std;

const int N = 1;
YagLaser::YagLaser(): m_connected(false)
{
    m_shutterStatus = false;
}

YagLaser::~YagLaser()
{
    this->CloseShutter();
    this->SetLaserOff();
}

bool YagLaser::Connect(int port)
{
    if(m_connected) {
        return true;
    }

    m_serialPortPtr = boost::shared_ptr<SerialPort>(new SerialPort());
    stringstream ss;
    ss << "COM" << port;

    bool ok = m_serialPortPtr->Open(ss.str().c_str(), 4800);
    if(!ok) {
        return false;
    }
    m_connected = true;
    return true;
}

bool YagLaser::Disconnect()
{
    if(!m_connected) {
        return true;
    }

    if(m_serialPortPtr.get()) {
        m_serialPortPtr->Close();
        m_connected = false;
    }
    return true;
}

bool YagLaser::OpenShutter()
{
    if(!m_connected) {
        return false;
    }

    for(int i = 0; i < N; i++) {
        string cmd("C1\r");
        Write(cmd);
        ReadResponse();
    }
    m_shutterStatus = true;
    return true;
}

bool YagLaser::CloseShutter()
{
    if(!m_connected) {
        return false;
    }

    for(int i = 0; i < N; i++) {
        string cmd("C0\r");
        Write(cmd);
        ReadResponse();
    }
    m_shutterStatus = false;
    return true;
}

bool YagLaser::SetLaserOn()
{
    if(!m_connected) {
        return false;
    }

    string cmd("B2\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SetLaserOff()
{
    if(!m_connected) {
        return false;
    }
    string cmd("B0\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SetLaserStandby()
{
    if(!m_connected) {
        return false;
    }
    string cmd("B1\r");

    Write(cmd);
    ReadResponse();
    return true;
}

string YagLaser::GetLaserStatus()
{
    if(!m_connected) {
        return "not connected";
    }
    string st;
    for(int i = 0; i < N; i++) {
        string cmd("O1\r");

        Write(cmd);
        st = ReadResponse();
        if(st == "OF") {
            st = "Off";        
        } else if(st == "ST") {
            st = "Standby";
        } else if(st == "ON") {
            st = "On";
        } else if(st == "S1") {
            st = "Standby_1";
        } else if(st == "S2") {
            st = "Standby_2";
        } else {
            st = "Unknown";
        }
    }
    return st;
}

string YagLaser::GetShutterStatus()
{
    if(!m_connected) {
        return "not connected";
    }
    string st;
    for(int i = 0; i < N; i++) {
        string cmd("O2\r");

        Write(cmd);
        st = ReadResponse();
        if(st == "OP") {
            st = "Open";
        } else if(st == "CL") {
            st = "Close";
        } else {
            st = "Unknown";
        }
    }
    return st;
}

string YagLaser::GetPowerPercent()
{
    if(!m_connected) {
        return "not connected";
    }
    string response;
    int power;
    for(int i = 0; i < N; i++) {
        string cmd("OA\r");
        Write(cmd);
        response = ReadResponse();

        try {
            power = boost::lexical_cast<int>(response);
            wxString s;
            s << power;
            response = s.c_str();
        } catch(boost::bad_lexical_cast &err) {
            response = "unknown";
        }
    }
    return response;
}

string YagLaser::GetPowerWatt()
{
    if(!m_connected) {
        return "not connected";
    }
    string response;

    for(int i = 0; i < N; i++) {
        string cmd("O3\r");
        Write(cmd);
        response = ReadResponse();
        try {
            int p = boost::lexical_cast<int>(response);
            wxString s;
            s << p;
            response = s.c_str();
        } catch(boost::bad_lexical_cast& err) {
            response = "unknown";
        }
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
    if(!m_connected) {
        return false;
    }
    string cmd("i0\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SingleShot()
{
    if(!m_connected) {
        return false;
    }
    string cmd("X\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SetPower(int percent)
{
    if(!m_connected) {
        return false;
    }
    //    stringstream ss;
    //    ss << "G" << percent;

    //    Write(ss.str());
    //    ReadResponse();
    m_daqboard->SetLaserPower(percent);
    return true;
}

bool YagLaser::SetEPCHighValue()
{
    if(!m_connected) {
        return false;
    }
    string cmd("g100\r");
    Write(cmd);
    ReadResponse();
    return true;
}

bool YagLaser::SetEPCLowValue()
{
    if(!m_connected) {
        return false;
    }
    string cmd("h0\r");
    Write(cmd);
    ReadResponse();
    return true;
}

string YagLaser::GetEPCStatus()
{
    if(!m_connected) {
        return "not connected";
    }
    string cmd("Oj\r");

    Write(cmd);
    string response = ReadResponse();

    try {
        int code = boost::lexical_cast<int>(response);
        string response;
        if(code == 0) {
            response = "Epc Off";
        } else if(code == 1) {
            response = "Epc On";
        } else {
            response = "Unknow";
        }
        return response;
    } catch(boost::bad_lexical_cast& err) {
        response = "unknown";
    }
    return response;
}

string YagLaser::ReadResponse()
{
    ::wxUsleep(10);
    string response;
    int len = m_serialPortPtr->Read(response);

    return response;
}

void YagLaser::Write(const string& cmd)
{
    int n = m_serialPortPtr->Write(cmd);
}

string YagLaser::GetInterlockStatus()
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

    int n = sizeof(Interlock)/sizeof(char*);
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

bool YagLaser::IsShutterOpen()
{
    return m_shutterStatus;   
}