#pragma once
#include <wx/wx.h>
class Map2D : public wxFrame
{
public:
    Map2D(wxWindow *parent, wxWindowID id = wxID_ANY);

private:
    int px, py;
    wxBitmap minimap;
    bool bmploaded = false;

    void OnPaint(wxPaintEvent &event);
    wxDECLARE_EVENT_TABLE();
};