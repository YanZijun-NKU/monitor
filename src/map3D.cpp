#include "map3D.h"
#include <wx/dcbuffer.h>
#include <algorithm>
#include "main.h"
// 在.h 中定义过的class不需要再次写class name:baseclass{}
// 直接写内部的函数实现
// extern GameWorld world;

static float PointToLineDist(wxPoint p, wxPoint l1, wxPoint l2)
{
    int x = p.x, y = p.y;
    int x1 = l1.x, y1 = l1.y;
    int x2 = l2.x, y2 = l2.y;

    float A = x - x1;
    float B = y - y1;
    float C = x2 - x1;
    float D = y2 - y1;

    float dot = A * C + B * D;
    float lenSq = C * C + D * D;

    if (lenSq < 0.001f)
        return hypot((double)(x - x1), (double)(y - y1));

    float t = dot / lenSq;
    if (t < 0.0f)
        return hypot((double)(x - x1), (double)(y - y1));
    if (t > 1.0f)
        return hypot((double)(x - x2), (double)(y - y2));

    float projX = x1 + t * C;
    float projY = y1 + t * D;
    return hypot((double)(x - projX), (double)(y - projY));
}

wxBEGIN_EVENT_TABLE(Map3D, wxPanel) // 前面写当前类，后面写父类
                                    // 从wxpanel传过来的event和对应函数绑定
    EVT_PAINT(Map3D::OnPaint)
        EVT_KEY_UP(Map3D::OnKeyUp)
            EVT_KEY_DOWN(Map3D::OnKeyDown)
                EVT_MOTION(Map3D::OnMouseMove)
    // EVT_TIMER(wxID_ANY, Map3D::OnTimer) // wxid——any任何一个定时器都可以触发
    EVT_LEFT_DOWN(Map3D::OnMouseDown)
        EVT_LEFT_UP(Map3D::OnMouseUp)
            wxEND_EVENT_TABLE();

Map3D::Map3D(wxWindow *parent, wxWindowID id)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS),
      m_parent(parent)
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

void Map3D::ClearCanvas()
{
    m_drawLines.clear();
    Refresh(false);
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

    if (!m_drawLines.empty())
    {
        dc.SetPen(wxPen(*wxBLACK, 2));
        for (auto &line : m_drawLines)
        {
            dc.DrawLine(line.s, line.e);
        }
    }
}

void Map3D::OnKeyDown(wxKeyEvent &evt)
{
    // 新增：ESC 键退出全屏（macOS 下画布里直接捕获）
    if (evt.GetKeyCode() == WXK_ESCAPE)
    {
        // 把 m_parent 强制转换为顶层窗口类型
        wxTopLevelWindow *topWin = dynamic_cast<wxTopLevelWindow *>(m_parent);
        if (topWin && topWin->IsFullScreen())
        {
            topWin->ShowFullScreen(false);
            return;
        }
    }
    world.input.SetKeyDown(evt.GetKeyCode());
    evt.Skip();
}

void Map3D::OnKeyUp(wxKeyEvent &evt)
{
    world.input.SetKeyUp(evt.GetKeyCode());
    evt.Skip();
}

void Map3D::OnMouseDown(wxMouseEvent &evt)
{
    if (m_drawMode == TOOL_LINE || m_drawMode == TOOL_ERASER)
    {
        m_isDrawing = true;
        m_lastDrawPt = evt.GetPosition();
    }
    evt.Skip();
}

void Map3D::OnMouseUp(wxMouseEvent &evt)
{
    m_isDrawing = false;
    // this->SetFocus();
    evt.Skip();
}

void Map3D::OnMouseMove(wxMouseEvent &evt)
{
    int x = evt.GetX();
    int y = evt.GetY();
    // wxLogDebug("map3dcurtool = %d\n",m_drawMode);
    //  普通模式：相机视角控制
    if (m_drawMode == TOOL_NONE)
    {
        static int LastX = -1;
        static int LastY = -1;

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
    }
    // 画线模式：记录线段
    else if (m_drawMode == TOOL_LINE)
    {
        if (evt.Dragging() && m_isDrawing)
        {
            m_drawLines.emplace_back(m_lastDrawPt, evt.GetPosition());
            m_lastDrawPt = evt.GetPosition();
        }
    }
    // 橡皮擦模式：精准删除选中线段
    else if (m_drawMode == TOOL_ERASER)
    {
        if (evt.Dragging())
        {
            wxPoint cur = evt.GetPosition();
            const float eraseThreshold = 8.0f;

            // 反向遍历安全删除
            for (auto it = m_drawLines.rbegin(); it != m_drawLines.rend(); ++it)
            {
                float dist = PointToLineDist(cur, it->s, it->e);
                if (dist < eraseThreshold)
                {
                    m_drawLines.erase(std::next(it).base());
                    break;
                }
            }
        }
    }

    evt.Skip();
}