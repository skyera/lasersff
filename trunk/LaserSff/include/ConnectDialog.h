#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <wx/wx.h>
#include <wx/combobox.h>

class ConnectDialog: public wxDialog
{
public:
    ConnectDialog(wxWindow *parent, const wxChar* title);
    int GetSerialPortNumber();
private:
    void OnOK(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()

    //
    wxComboBox* m_box;
    int m_serialPort;
};

#endif