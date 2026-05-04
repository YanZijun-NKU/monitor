#pragma once
#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>

class MainFrame;

enum GameToolID
{
    TOOL_NONE = 0,
    TOOL_LINE = 2001,
    TOOL_ERASER,
    TOOL_ZOOMIN,
    TOOL_ZOOMOUT
};

class Mytoolbar : public wxToolBar
{
public:
    Mytoolbar(wxWindow *parent, wxWindowID id = wxID_ANY);
    // int GetCurrentTool() const { return curtool; }
    //void OnToolClick(wxCommandEvent &e);

private:
    MainFrame *g_parent;
    int curtool;

    // wxDECLARE_EVENT_TABLE();
};