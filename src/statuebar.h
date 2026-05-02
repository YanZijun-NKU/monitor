#pragma once

#include <wx/wx.h>
#include <wx/statusbr.h>
#include <wx/stopwatch.h>
#include <wx/timer.h>

class Mystatusbar : public wxStatusBar
{
public:
    explicit Mystatusbar(wxWindow *parent);
    ~Mystatusbar() noexcept override; // 消除报错
    void SetFps(int fps);
    void SetCameraPos(float x, float y, float z);
    void UpdateRuntime();
    void Ontimer(wxTimerEvent &e);
    void TickFrame();

private:
    enum StatusFields
    {
        FILED_READY = 0,
        FILED_RUNTIME,
        FILED_FPS,
        FILED_POS,
        FILED_COUNT
    };
    wxStopWatch stopwatch;
    wxTimer m_timer;
    int m_framecnt = 0;
};