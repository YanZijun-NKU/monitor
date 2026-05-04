#pragma once
#include <wx/wx.h>
#include <wx/timer.h>
#include "gameworld.h"
#include "render.h"
#include "map2D.h"
#include "statuebar.h"
#include "toolbar.h"
#include <vector>
#include <cmath>

class MainFrame;
// 实际是子窗口的交互接口
struct DrawLine
{
    wxPoint s;
    wxPoint e;
    DrawLine(wxPoint s_, wxPoint e_) : s(s_), e(e_) {}
};

class Map3D : public wxPanel
{
public:
    Map3D(wxWindow *parent, wxWindowID id = wxID_ANY);
    // parent是必须的，必须知道自己的父窗口是谁，否则交互、关闭时将发生错误
    ~Map3D();
    void SetStatusBar(Mystatusbar *bar)
    {
        m_statusbar = bar;
    }
    void SetMap2D(Map2D *p2d)
    {
        m_map2d = p2d;
    }
    void GetDrawmode(int mode)
    {
        m_drawMode = mode;
        if (mode == TOOL_NONE)
        {
            m_isDrawing = 0;
        }
    }
    void ClearCanvas();
    void ZoomIn()
    {
        render->ZoomIn();
        Refresh();
    }
    void ZoomOut()
    {
        render->ZoomOut();
        Refresh();
    }

private:
    int m_drawMode = TOOL_NONE;
    bool m_isDrawing = false;
    wxPoint m_lastDrawPt;
    std::vector<DrawLine> m_drawLines;
    wxWindow *m_parent;

    wxTimer timer; // 帧率刷新计时器
    Render *render = nullptr;
    Map2D *m_map2d = nullptr;
    Mystatusbar *m_statusbar = nullptr;

    // wxevent:系统发送的交互事件包，必须接收
    void OnTimer(wxTimerEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);
    void OnMouseMove(wxMouseEvent &event);
    void OnMouseUp(wxMouseEvent &event);
    void OnMouseDown(wxMouseEvent &event);

    wxDECLARE_EVENT_TABLE();
};