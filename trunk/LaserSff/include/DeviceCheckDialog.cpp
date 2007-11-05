#include "DeviceCheckDialog.h"

BEGIN_EVENT_TABLE(DeviceCheckDialog, wxDialog)
    EVT_BUTTON(wxID_OK, DeviceCheckDialog::OnOK)
END_EVENT_TABLE()


DeviceCheckDialog::DeviceCheckDialog(wxWindow *parent, const wxString title)
    :wxDialog(parent, wxID_ANY, title)
{
    m_run = false;
    wxString items[] = {"Laser On", "Cooling water", "Protection Gas", 
        "Carrier gas", "Powder on"};
    m_checkListBox = new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, 
                                        wxSize(100, 100),    
                                        WXSIZEOF(items), items);

    for(int i = 0; i < m_checkListBox->GetCount(); i += 2) {
        m_checkListBox->GetItem(i)->SetBackgroundColour(*wxLIGHT_GREY);
    }
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_checkListBox, wxSizerFlags(1).Expand().Border(wxALL, 5));
    wxSizer *btns = CreateButtonSizer(wxOK|wxCANCEL);
    sizer->Add(btns, wxSizerFlags().Center().Border(wxALL, 5));
    SetSizer(sizer);
    sizer->Fit(this);
    CenterOnParent();
}

DeviceCheckDialog::~DeviceCheckDialog()
{

}

void DeviceCheckDialog::OnOK(wxCommandEvent& event)
{
    m_run = true;
    for(int i = 0; i < m_checkListBox->GetCount(); i++) {
        bool flag = m_checkListBox->IsChecked(i);
        if(!flag) {
            m_run = false;
            break;
        }
    }    
    EndDialog(wxID_OK);
}

bool DeviceCheckDialog::IsRun()
{
    return m_run;
}