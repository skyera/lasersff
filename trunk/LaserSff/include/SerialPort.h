#ifndef RCAM_SERIAL_PORT_H
#define RCAM_SERIAL_PORT_H
#include <wx/wx.h>
#include <string>
namespace rcam {

class SerialPort
{
public:
	SerialPort();
	virtual ~SerialPort();

	bool Open(const std::string& port, int baud);
	bool Close();

	int Read(std::string& str);
	int Write(const std::string& str);

private:
	bool m_opened;
	HANDLE m_hcom;

};
}
#endif