#ifndef DEVICE_CHECK_DIALOG_H
#define DEVICE_CHECK_DIALOG_H

#include <wx/wx.h>
#include <wx/checklst.h>

class DeviceCheckDialog: public wxDialog
{
public:
    DeviceCheckDialog(wxWindow *parent, const wxString title);
    ~DeviceCheckDialog();
    bool IsRun();
    void OnOK(wxCommandEvent& event);
private:
    wxCheckListBox *m_checkListBox;
    bool m_run;

    DECLARE_EVENT_TABLE()

};

#endif