#include "statuebar.h"
Mystatusbar::Mystatusbar(wxWindow *parent) : wxStatusBar(parent, wxID_ANY), m_timer(this)
{
    int widths[FILED_COUNT] = {-1, 160, 80, 160};

    Bind(wxEVT_TIMER, &Mystatusbar::Ontimer, this);
    m_timer.Start(1000);
    stopwatch.Start();

    SetFieldsCount(FILED_COUNT, widths);
    SetStatusText("就绪", FILED_READY);
    SetFps(0);
    SetCameraPos(0, 0, 0);
    UpdateRuntime();

} // 构造函数只做一件事，初始化。
Mystatusbar::~Mystatusbar() noexcept
{
    m_timer.Stop();
}

void Mystatusbar::SetFps(int fps)
{
    wxString text = wxString::Format("FPS:%d", fps);
    SetStatusText(text, FILED_FPS);
}
void Mystatusbar::SetCameraPos(float x, float y, float z)
{
    wxString text = wxString::Format("Pos:%.2f,%.2f,%.2f", x, y, z);
    SetStatusText(text, FILED_POS);
}
void Mystatusbar::UpdateRuntime()
{
    long totalSec = stopwatch.Time() / 1000;
    int h = totalSec / 3600;
    int m = (totalSec % 3600) / 60;
    int s = totalSec % 60;
    wxString text = wxString::Format("%02d:%02d:%02d", h, m, s);
    SetStatusText(text, FILED_RUNTIME);
}

void Mystatusbar::Ontimer(wxTimerEvent &e)
{
    UpdateRuntime();
    SetFps(m_framecnt);
    m_framecnt = 0;
}

void Mystatusbar::TickFrame()
{
    m_framecnt++;
    // wxLogDebug("framecnt:%d", m_framecnt);
    //  wxLongLong now=wxGetLocalTimeMillis();
}