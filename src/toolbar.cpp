#include "toolbar.h"
#include "main.h"

Mytoolbar::Mytoolbar(wxWindow *parent, wxWindowID id) : wxToolBar(parent, id, wxDefaultPosition, wxDefaultSize, wxTB_VERTICAL | wxTB_TEXT), curtool(TOOL_NONE)
{
    g_parent = dynamic_cast<MainFrame *>(parent);
    // Bind(wxEVT_COMMAND_TOOL_CLICKED, &Mytoolbar::OnToolClick, this);

    AddTool(TOOL_NONE, "cursor", wxArtProvider::GetBitmap(wxART_GO_HOME), "no tool", wxITEM_RADIO);
    AddTool(TOOL_LINE, "pencil", wxArtProvider::GetBitmap(wxART_EDIT), "pencil", wxITEM_RADIO);
    AddTool(TOOL_ERASER, "eraser", wxArtProvider::GetBitmap(wxART_DELETE), "clear", wxITEM_RADIO);
    AddSeparator();
    AddTool(TOOL_ZOOMIN, "zoomin", wxArtProvider::GetBitmap(wxART_PLUS), "zoomin", wxITEM_NORMAL);
    AddTool(TOOL_ZOOMOUT, "zoomout", wxArtProvider::GetBitmap(wxART_MINUS), "zoomout", wxITEM_NORMAL);

    // wxLogDebug("tot:%zu\n", GetToolsCount());
    Realize();
}