#include "map3D.h"
#include <wx/dcbuffer.h>
// 在.h 中定义过的class不需要再次写class name:baseclass{}
// 直接写内部的函数实现
// extern GameWorld world;

wxBEGIN_EVENT_TABLE(Map3D, wxPanel) // 前面写当前类，后面写父类
                                    // 从wxpanel传过来的event和对应函数绑定
    EVT_PAINT(Map3D::OnPaint)
        EVT_KEY_UP(Map3D::OnKeyUp)
            EVT_KEY_DOWN(Map3D::OnKeyDown)
                EVT_MOTION(Map3D::OnMouseMove)
    // EVT_TIMER(wxID_ANY, Map3D::OnTimer) // wxid——any任何一个定时器都可以触发
    wxEND_EVENT_TABLE();

Map3D::Map3D(wxWindow *parent, wxWindowID id)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetBackgroundColour(*wxBLACK);
    SetFocus();      // 接收键盘输入
    timer.Start(32); // 30fps
    timer.Bind(wxEVT_TIMER, &Map3D::OnTimer, this);
    // wxPrintf("map3d created!\n");
}

Map3D::~Map3D()
{
    timer.Stop();
    delete render;
}

void Map3D::OnTimer(wxTimerEvent &event)
{
    static wxLongLong lastTime = 0;
    wxLongLong now = wxGetLocalTimeMillis();
    float dt = (now - lastTime).ToDouble() / 1000.0f;
    lastTime = now;
    // printf("ontimer is refreshed!\n");
    world.Update(dt);
    this->Refresh(false);
    if (m_map2d)
    {
        m_map2d->Refresh(false);
    }
    // refresh == onpaint(wxpaintevent)
    if (m_statusbar)
    {
        m_statusbar->SetCameraPos(world.player.x, world.player.y, world.player.z);
        m_statusbar->TickFrame();
    }
}

void Map3D::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    // dc.SetPen(*wxRED_PEN);
    // dc.DrawCircle(100, 100, 50);

    wxSize size = GetClientSize();
    int w = size.GetWidth();
    int h = size.GetHeight();
    // draw
    if (!render)
    {
        // wxPrintf("render is null\n");
        if (w > 0 && h > 0)
        {
            render = new Render(w, h);
        }
        else
            return;
    }
    else
    {
        render->Resize(w, h);
    }
    Vec3 ppos = Vec3(world.player.x, world.player.y, world.player.z);
    render->SetCamera(ppos, world.player.yaw, world.player.pitch);
    render->DrawHouse(dc);
    render->DrawDoor(dc, world.house.Dooropen);
    render->DrawWindows(dc, world.house.Windowsopen);
}

void Map3D::OnKeyDown(wxKeyEvent &evt)
{
    world.input.SetKeyDown(evt.GetKeyCode());
    evt.Skip();
}

void Map3D::OnKeyUp(wxKeyEvent &evt)
{
    world.input.SetKeyUp(evt.GetKeyCode());
    evt.Skip();
}

void Map3D::OnMouseMove(wxMouseEvent &evt)
{
    static int LastX = -1;
    static int LastY = -1;

    int x = evt.GetX();
    int y = evt.GetY();

    if (LastX != -1 && LastY != -1 && evt.Dragging())
    {
        world.player.yaw += (x - LastX) * world.player.rotspeed;
        world.player.pitch += (y - LastY) * world.player.rotspeed;
        if (world.player.pitch > 1.4f)
            world.player.pitch = 1.4f;
        if (world.player.pitch < -1.4f)
            world.player.pitch = -1.4f;
    }
    LastX = x;
    LastY = y;
    evt.Skip();
}
