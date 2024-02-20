#pragma once

#include <wx/wx.h>
#include <wx/filesys.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/fs_zip.h>
#include <wx/progdlg.h>
#include <memory>

struct UnZipPanel : public wxPanel
{
    UnZipPanel(wxWindow *parent);

    wxBoxSizer *mainSizer;

    wxTextCtrl *zipFileTextCtrl;
    wxTextCtrl *outputDirTextCtrl;
    wxTextCtrl *singleFileTextCtrl;
    wxCheckBox *singleFileCheckBox;

    void InitializeUnZipSection();

    void ExecuteUnZip();
    void UnZipAll();
    void UnZipSingle();
};

UnZipPanel::UnZipPanel(wxWindow *parent) : wxPanel(parent)
{
    mainSizer = new wxBoxSizer(wxVERTICAL);

    InitializeUnZipSection();

    SetSizer(mainSizer);

    wxFileSystem::AddHandler(new wxZipFSHandler);
}

void UnZipPanel::InitializeUnZipSection()
{
    auto unzipTitleLabel = new wxStaticText(this, wxID_ANY, "Unzip File");
    unzipTitleLabel->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    zipFileTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    zipFileTextCtrl->SetEditable(false);

    auto browseZipFileButton = new wxButton(this, wxID_ANY, "Browse...");

    auto outputDirLabel = new wxStaticText(this, wxID_ANY, "Output Directory");
    outputDirLabel->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    outputDirTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    outputDirTextCtrl->SetEditable(false);

    auto setOutputDirButton = new wxButton(this, wxID_ANY, "Browse...");

    singleFileCheckBox = new wxCheckBox(this, wxID_ANY, "Only one file");
    singleFileTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    singleFileTextCtrl->Enable(false);

    auto unzipButton = new wxButton(this, wxID_ANY, "Unzip");

    auto unzipSizer = new wxBoxSizer(wxHORIZONTAL);
    unzipSizer->Add(zipFileTextCtrl, 1, wxEXPAND | wxRIGHT, FromDIP(8));
    unzipSizer->Add(browseZipFileButton, 0, wxEXPAND, FromDIP(8));

    auto outputDirSizer = new wxBoxSizer(wxHORIZONTAL);
    outputDirSizer->Add(outputDirTextCtrl, 1, wxEXPAND | wxRIGHT, FromDIP(8));
    outputDirSizer->Add(setOutputDirButton, 0, wxEXPAND, FromDIP(8));

    mainSizer->Add(unzipTitleLabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));
    mainSizer->Add(unzipSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));

    mainSizer->Add(outputDirLabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));
    mainSizer->Add(outputDirSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));

    mainSizer->AddSpacer(FromDIP(8));

    mainSizer->Add(singleFileCheckBox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));
    mainSizer->Add(singleFileTextCtrl, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));

    mainSizer->AddSpacer(FromDIP(8));

    mainSizer->Add(unzipButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(8));

    browseZipFileButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                              {
        wxFileDialog openFileDialog(this, "Open zip file", "", "", "Zip files (*.zip)|*.zip", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openFileDialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        zipFileTextCtrl->SetValue(openFileDialog.GetPath()); });

    setOutputDirButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                             {
        wxDirDialog dirDialog(this, "Select output directory", "", wxDD_DEFAULT_STYLE | wxDD_NEW_DIR_BUTTON);

        if (dirDialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        outputDirTextCtrl->SetValue(dirDialog.GetPath()); });

    singleFileCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent &event)
                             {
        singleFileTextCtrl->Enable(event.IsChecked());

        if (event.IsChecked())
        {
            singleFileTextCtrl->SetFocus();
        } });

    unzipButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                      { ExecuteUnZip(); });
}

void UnZipPanel::ExecuteUnZip()
{
    if (zipFileTextCtrl->GetValue().IsEmpty() || outputDirTextCtrl->GetValue().IsEmpty())
    {
        wxMessageBox("Please fill in all the fields.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (singleFileCheckBox->IsChecked())
    {
        UnZipSingle();
    }
    else
    {
        UnZipAll();
    }
}

void UnZipPanel::UnZipAll()
{
    wxProgressDialog progressDialog("Unzipping", "Unzipping file...", 100, this, wxPD_APP_MODAL | wxPD_AUTO_HIDE);

    wxFileInputStream inStream(zipFileTextCtrl->GetValue());
    wxZipInputStream zipIn(inStream);
    std::unique_ptr<wxZipEntry> entry(zipIn.GetNextEntry());

    while (entry)
    {
        progressDialog.Pulse();

        wxString entryName = entry->GetName();

        wxFileName destFileName = outputDirTextCtrl->GetValue() + wxFileName::GetPathSeparator() + entryName;
        bool isFile = !entry->IsDir();

        if (!wxDirExists(destFileName.GetPath()))
        {
            wxFileName::Mkdir(destFileName.GetPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
        }

        if (isFile)
        {
            if (!zipIn.CanRead())
            {
                wxLogError("Couldn't read the zip entry '%s'.", entry->GetName());
                return;
            }

            wxFileOutputStream outStream(destFileName.GetFullPath());

            if (!outStream.IsOk())
            {
                wxLogError("Couldn't create the file '%s'.", destFileName.GetFullPath());
                return;
            }

            zipIn.Read(outStream);

            zipIn.CloseEntry();
            outStream.Close();
        }

        entry.reset(zipIn.GetNextEntry());
    }
}

void UnZipPanel::UnZipSingle()
{
    if (singleFileTextCtrl->GetValue().IsEmpty())
    {
        wxMessageBox("Please enter a file name to extract.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxFileSystem fs;
    std::unique_ptr<wxFSFile> zip(fs.OpenFile(zipFileTextCtrl->GetValue() + "#zip:" + singleFileTextCtrl->GetValue()));

    if (zip)
    {
        wxInputStream *in = zip->GetStream();

        if (in)
        {
            wxFileName destFileName = outputDirTextCtrl->GetValue() + wxFileName::GetPathSeparator() + singleFileTextCtrl->GetValue();
            if (!wxDirExists(destFileName.GetPath()))
            {
                wxFileName::Mkdir(destFileName.GetPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
            }

            wxFileOutputStream out(destFileName.GetFullPath());
            out.Write(*in);
            out.Close();
        }
    }
    else
    {
        wxMessageBox("File not found in zip.", "Error", wxOK | wxICON_ERROR);
    }
}
