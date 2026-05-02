#include "map2D.h"
#include "gameworld.h"
#include <cmath>
#include <wx/dcbuffer.h>

// extern GameWorld world;

wxBEGIN_EVENT_TABLE(Map2D, wxFrame)
    EVT_PAINT(Map2D::OnPaint)
        wxEND_EVENT_TABLE();

Map2D::Map2D(wxWindow *parent, wxWindowID id) : wxFrame(parent, id, "2d小地图", wxDefaultPosition, wxSize(350, 350))
{
    minimap.LoadFile("game2D.png", wxBITMAP_TYPE_PNG); // address need to loaded
    bmploaded = minimap.IsOk();
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetBackgroundColour(*wxWHITE);
    Move(1000, 100);
}

void Map2D::OnPaint(wxPaintEvent &evt)
{
    // evt是触发重绘的信号
    wxAutoBufferedPaintDC dc(this);
    int w = GetClientSize().x;
    int h = GetClientSize().y;
    int MinimapSize = 250;
    int mmx = (w - MinimapSize) / 2;
    int mmy = (w - MinimapSize) / 2; // margin

    if (bmploaded)
    {
        dc.DrawBitmap(minimap, mmx, mmy);
    }
    else
    {
        dc.SetBrush(wxColor(255, 255, 255));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(mmx, mmy, MinimapSize, MinimapSize);
        wxMessageBox(wxT("二维地图加载失败！"), wxT("警告"), wxOK | wxICON_WARNING);
    }
    // draw dot
    float sceneCenterX = world.house.cx;
    float sceneCenterY = world.house.cy;
    float worldRange = world.worldrange;
    float scale = (float)MinimapSize / worldRange;
    px = mmx + MinimapSize / 2 + (world.player.x - sceneCenterX) * scale;
    py = mmy + MinimapSize / 2 + (world.player.y - sceneCenterY) * scale;

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(px - 16, py - 16, 32, 32);

    dc.SetBrush(wxColor(255, 50, 50));
    dc.DrawCircle(px, py, 5);

    float DirX = cos(world.player.yaw);
    float DirY = sin(world.player.yaw);
    dc.SetPen(wxPen(wxColor(255, 150, 150), 2));
    dc.DrawLine(px, py, px + DirX * 13.f, py + DirY * 13.f);
    dc.DrawLine(px + DirX * 13.f, py + DirY * 13.f, px + DirX * 11.f - DirY * 2.f, py + DirY * 11.f + DirX * 2.f);
    dc.DrawLine(px + DirX * 13.f, py + DirY * 13.f, px + DirX * 11.f + DirY * 2.f, py + DirY * 11.f - DirX * 2.f);
}
// 新建窗口，就继承frame；新建画板，就继承panel；要建什么，先在父窗口里面新建指针，再new一个派生类，然后手动show
// 构造函数只负责初始化，不负责绘画