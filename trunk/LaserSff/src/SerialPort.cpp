#include "SerialPort.h"

using namespace rcam;
using namespace std;

SerialPort::SerialPort()
{
    m_opened = false;
    m_hcom = NULL;
}

SerialPort::~SerialPort()
{
    Close();
}

bool SerialPort::Open(const string& port, int baud)
{
    if(m_opened) {
        return true;
    }	

    if(port.find("COM") == string::npos) {
        string msg = port + " does not contain COM: COM1...";
        throw std::exception(msg.c_str());
    }
    
    if(baud <= 0) {
        string msg = "baud rate > 0";
        throw std::exception(msg.c_str());
    }

    m_hcom = CreateFile(port.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(m_hcom == INVALID_HANDLE_VALUE) {
        return false;
    }

    COMMTIMEOUTS TimeOuts;

    GetCommTimeouts(m_hcom, &TimeOuts);

    TimeOuts.ReadIntervalTimeout = 15;
    TimeOuts.ReadTotalTimeoutMultiplier = 0;
    TimeOuts.ReadTotalTimeoutConstant = 1000;

    // Set write timeout to set the GetOverlappedResult's waiting time
    TimeOuts.WriteTotalTimeoutMultiplier = 0;
    TimeOuts.WriteTotalTimeoutConstant = 1000;
    SetCommTimeouts(m_hcom, &TimeOuts);

    // Set buffer for in, out
    SetupComm(m_hcom, 1024, 1024);
    SetCommMask(m_hcom, EV_RXCHAR);

    // Set com state
    DCB dcb;
    GetCommState(m_hcom, &dcb);
    dcb.BaudRate = baud; //4800;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(m_hcom, &dcb);

    m_opened = true;
    return true;
}

bool SerialPort::Close()
{
    if(m_opened) {
        CloseHandle(m_hcom);
        m_opened = false;
        m_hcom = NULL;
    }
    return true;
}

int SerialPort::Write(const std::string &str)
{
    if(!m_opened) {
        return 0;
    }
    PurgeComm(m_hcom, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);   

    int size = str.size();
    DWORD length;
    WriteFile(m_hcom, str.c_str(), size, &length, NULL);
    return length;
}

int SerialPort::Read(std::string &str)
{
    if(!m_opened) {
        return -1;
    }    
    DWORD ErrorFlags;
    COMSTAT ComStat;
    ClearCommError(m_hcom, &ErrorFlags, &ComStat);

    char buf[200];
    DWORD length = 200;
    ReadFile(m_hcom, buf, length, &length, NULL);
    if(length == 0) {
        buf[0] = 0;
    } else if(buf[length - 1] == 'F') {
        buf[length-1] = 0;
    } else {
        buf[length] = 0;
    }

    str = buf;
    return length;
}