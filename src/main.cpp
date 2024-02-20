#include <wx/wx.h>
#include <wx/notebook.h>
#include "zippanel.h"
#include "unzippanel.h"

class Application : public wxApp
{
public:
    virtual bool Initialize();
};

wxIMPLEMENT_APP(Application);

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &position, const wxSize &size);
};

bool Application::Initialize()
{
    MainFrame *frame = new MainFrame("Zip Utility", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString &title, const wxPoint &position, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, position, size)
{
    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    auto notebook = new wxNotebook(this, wxID_ANY);
    notebook->SetInternalBorder(0);
    notebook->AddPage(new ZipPanel(notebook), "Zip");
    notebook->AddPage(new UnZipPanel(notebook), "UnZip");

    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, FromDIP(10));
    this->SetSizer(mainSizer);

    this->SetSize(FromDIP(wxSize(700, 600)));
    this->SetMinSize(FromDIP(wxSize(600, 500)));

    this->SetBackgroundColour(notebook->GetBackgroundColour());
}
