#pragma once
#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>

enum GameToolID
{
    TOOL_NONE = 0,
    TOOL_1 = 2001,
    TOOL_2,
    TOOL_3,
    TOOL_4
};

class Mytoolbar : public wxToolBar
{
public:
    Mytoolbar(wxWindow *parent, wxWindowID id = wxID_ANY);
    int GetCurrentTool() const { return curtool; }

private:
    int curtool;
    void OnToolClick(wxCommandEvent &e);
    wxDECLARE_EVENT_TABLE();
};