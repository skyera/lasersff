#include "ConnectDialog.h"
#include <wx/statline.h>

BEGIN_EVENT_TABLE(ConnectDialog, wxDialog)
    EVT_BUTTON(wxID_OK, ConnectDialog::OnOK)
END_EVENT_TABLE()

ConnectDialog::ConnectDialog(wxWindow* parent, const wxChar* title)
    :wxDialog(parent, wxID_ANY, wxString(title))
{
    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *top_sizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _T("Communication")), wxHORIZONTAL);
    wxBoxSizer *bot_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    main_sizer->Add(top_sizer, wxSizerFlags().Expand().Border(wxALL, 5));
    main_sizer->Add(new wxStaticLine(this, -1, wxDefaultPosition, wxSize(2, 2),
        wxHORIZONTAL),wxSizerFlags().Expand());
    main_sizer->AddSpacer(5);
    main_sizer->Add(bot_sizer, wxSizerFlags().Expand().Border(wxALL, 5));
    main_sizer->AddSpacer(5);
    
    top_sizer->Add(new wxStaticText(this, -1, _T("Serial Port")), wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL, 5));
    
    wxString items[] = {_T("COM1"), _T("COM2"), _T("COM3"), _T("COM4"), _T("COM5"), _T("COM6") };
    m_box = new wxComboBox(this, -1, _T("COM1"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(items), items);
    m_box->SetSelection(2);
    top_sizer->Add(m_box, 1, wxALL|wxGROW, 5);
    
    
    bot_sizer->Add(new wxButton(this, wxID_OK, _T("OK")),wxSizerFlags().Border(wxALL, 5));
    bot_sizer->AddSpacer(20);
    bot_sizer->Add(new wxButton(this, wxID_CANCEL, _T("Cancel")),wxSizerFlags().Border(wxALL, 5));
    SetSizer(main_sizer);
    Layout();
    main_sizer->Fit(this);
    CenterOnParent();
}

void ConnectDialog::OnOK(wxCommandEvent& event)
{
    m_serialPort = m_box->GetCurrentSelection() + 1;
    EndDialog(wxID_OK);
}

int ConnectDialog::GetSerialPortNumber()
{
    return m_serialPort;
}