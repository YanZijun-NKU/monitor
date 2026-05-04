#include "toolbar.h"
wxBEGIN_EVENT_TABLE(Mytoolbar, wxToolBar)
    EVT_TOOL_RANGE(TOOL_LINE, TOOL_4, Mytoolbar::OnToolClick)
        wxEND_EVENT_TABLE()

            Mytoolbar::Mytoolbar(wxWindow *parent, wxWindowID id) : wxToolBar(parent, id, wxDefaultPosition, wxDefaultSize, wxTB_VERTICAL | wxTB_TEXT), curtool(TOOL_NONE)
{
    AddTool(TOOL_LINE, "pencil", wxArtProvider::GetBitmap(wxART_EDIT), "pencil", wxITEM_RADIO);
    AddTool(TOOL_ERASER, "eraser", wxArtProvider::GetBitmap(wxART_DELETE), "clear", wxITEM_RADIO);
    AddTool(TOOL_3, "name3", wxArtProvider::GetBitmap(wxART_TIP), "tool3_test", wxITEM_RADIO);
    AddTool(TOOL_4, "name4", wxArtProvider::GetBitmap(wxART_CUT), "tool4_test", wxITEM_RADIO);
    Realize();
}
void Mytoolbar::OnToolClick(wxCommandEvent &e)
{
    int id = e.GetId();
    if (id == curtool)
    {
        ToggleTool(curtool, false);
        curtool = TOOL_NONE;
        return;
    }
    if (curtool != TOOL_NONE)
    {
        ToggleTool(curtool, false);
        curtool = id;
        ToggleTool(curtool, true);
    }
}