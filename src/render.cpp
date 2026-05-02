#include "render.h"

Render::Render(int w, int h)
{
    // initialization
    m_fov = 1.05f;
    m_near = 0.1f;
    m_winW = w;
    m_winH = h;
}

void Render::SetCamera(Vec3 c, float y, float p)
{
    Campos = c;
    yaw = y;
    pitch = p;
}

wxPoint Render::WorldToScr(Vec3 worldPt)
{
    // worldpt 是任意点,把绝对坐标变换为相对坐标,再实现2d投影
    float dx1 = worldPt.x - Campos.x;
    float dy1 = worldPt.y - Campos.y;
    float dz1 = worldPt.z - Campos.z;
    // xuanzhuan-1
    float dx2 = dx1 * cosf(-yaw) - dy1 * sinf(-yaw);
    float dy2 = dy1 * cosf(-yaw) + dx1 * sinf(-yaw);
    float dz2 = dz1;
    // xuanzhuan-2,y axis is forward
    float dx = dx2;
    float dy = dy2 * cosf(-pitch) - dz2 * sinf(-pitch);
    float dz = dy2 * sinf(-pitch) + dz2 * cosf(-pitch);
    // project to scr(2d)
    if (dy < m_near)
        dy = m_near;
    float scale = 1.0f / tanf(m_fov * 0.5f);
    float px = scale * dx / dy;
    float py = scale * dz / dy;
    // make center
    int sx, sy;
    sx = (int)(m_winW * 0.5f + px * m_winW * 0.5f);
    sy = (int)(m_winH * 0.5f - py * m_winH * 0.5f);
    return wxPoint(sx, sy);
}

void Render::DrawQuad(wxPaintDC &dc, Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4)
{
    wxPoint sp1 = WorldToScr(p1);
    wxPoint sp2 = WorldToScr(p2);
    wxPoint sp3 = WorldToScr(p3);
    wxPoint sp4 = WorldToScr(p4);
    if (sp1.x < 0 && sp2.x < 0 && sp3.x < 0 && sp4.x < 0)
        return;
    if (sp1.y < 0 && sp2.y < 0 && sp3.y < 0 && sp4.y < 0)
        return;
    if (sp1.x > m_winW && sp2.x > m_winW && sp3.x > m_winW && sp4.x > m_winW)
        return;
    if (sp1.y > m_winH && sp2.y > m_winH && sp3.y > m_winH && sp4.y > m_winH)
        return;
    dc.DrawLine(sp1, sp2);
    dc.DrawLine(sp2, sp3);
    dc.DrawLine(sp3, sp4);
    dc.DrawLine(sp4, sp1);
}

static const float HOUSE_X = 320.0;
static const float HOUSE_Y = 320.0;
static const float HOUSE_BASE_Z = 0.0;

void Render::DrawHouse(wxPaintDC &dc)
{
    // dc.SetPen(wxPen(*wxWHITE,2));
    // dc.DrawLine(10,10,200,200);
    //  地面墙体示例（X-Y地面，Z高度）
    Vec3 v1(HOUSE_X - 8, HOUSE_Y - 8, HOUSE_BASE_Z);
    Vec3 v2(HOUSE_X + 8, HOUSE_Y - 8, HOUSE_BASE_Z);
    Vec3 v3(HOUSE_X + 8, HOUSE_Y - 8, HOUSE_BASE_Z + 6);
    Vec3 v4(HOUSE_X - 8, HOUSE_Y - 8, HOUSE_BASE_Z + 6);
    DrawQuad(dc, v1, v2, v3, v4);
}

// 门绘制：开关状态区分
void Render::DrawDoor(wxPaintDC &dc, bool isOpen)
{
    if (!isOpen)
    {
        // 关门：垂直墙面
        Vec3 d1(HOUSE_X - 2, HOUSE_Y - 8, HOUSE_BASE_Z);
        Vec3 d2(HOUSE_X + 2, HOUSE_Y - 8, HOUSE_BASE_Z);
        Vec3 d3(HOUSE_X + 2, HOUSE_Y - 8, HOUSE_BASE_Z + 3.2f);
        Vec3 d4(HOUSE_X - 2, HOUSE_Y - 8, HOUSE_BASE_Z + 3.2f);
        DrawQuad(dc, d1, d2, d3, d4);
    }
    else
    {
        // 开门：沿X轴向侧翻开
        Vec3 d1(HOUSE_X - 2, HOUSE_Y - 8, HOUSE_BASE_Z);
        Vec3 d2(HOUSE_X - 2, HOUSE_Y - 4, HOUSE_BASE_Z);
        Vec3 d3(HOUSE_X - 2, HOUSE_Y - 4, HOUSE_BASE_Z + 3.2f);
        Vec3 d4(HOUSE_X - 2, HOUSE_Y - 8, HOUSE_BASE_Z + 3.2f);
        DrawQuad(dc, d1, d2, d3, d4);
    }
}

// 窗户绘制
void Render::DrawWindows(wxPaintDC &dc, bool isOpen)
{
    if (!isOpen)
    {
        Vec3 w1(HOUSE_X + 4, HOUSE_Y - 8, HOUSE_BASE_Z + 2);
        Vec3 w2(HOUSE_X + 7, HOUSE_Y - 8, HOUSE_BASE_Z + 2);
        Vec3 w3(HOUSE_X + 7, HOUSE_Y - 8, HOUSE_BASE_Z + 3.5f);
        Vec3 w4(HOUSE_X + 4, HOUSE_Y - 8, HOUSE_BASE_Z + 3.5f);
        DrawQuad(dc, w1, w2, w3, w4);
    }
    else
    {
        Vec3 w1(HOUSE_X + 4, HOUSE_Y - 8, HOUSE_BASE_Z + 2);
        Vec3 w2(HOUSE_X + 4, HOUSE_Y - 5, HOUSE_BASE_Z + 2);
        Vec3 w3(HOUSE_X + 4, HOUSE_Y - 5, HOUSE_BASE_Z + 3.5f);
        Vec3 w4(HOUSE_X + 4, HOUSE_Y - 8, HOUSE_BASE_Z + 3.5f);
        DrawQuad(dc, w1, w2, w3, w4);
    }
}

void Render::Resize(int w, int h)
{
    m_winW = w;
    m_winH = h;
}