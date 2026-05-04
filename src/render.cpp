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

void Render::ZoomIn()
{
    m_fov = std::max(m_minfov, m_fov * 0.95f);
    wxLogDebug("zoomin:%.3f\n", m_fov);
}

void Render::ZoomOut()
{
    m_fov = std::min(m_maxfov, m_fov * 1.05f);
    wxLogDebug("zoomout:%.3f\n", m_fov);
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

void Render::DrawTriangle(wxPaintDC &dc, Vec3 p1, Vec3 p2, Vec3 p3)
{
    wxPoint sp1 = WorldToScr(p1);
    wxPoint sp2 = WorldToScr(p2);
    wxPoint sp3 = WorldToScr(p3);

    if (sp1.x < 0 && sp2.x < 0 && sp3.x < 0)
        return;
    if (sp1.y < 0 && sp2.y < 0 && sp3.y < 0)
        return;
    if (sp1.x > m_winW && sp2.x > m_winW && sp3.x > m_winW)
        return;
    if (sp1.y > m_winH && sp2.y > m_winH && sp3.y > m_winH)
        return;

    dc.DrawLine(sp1, sp2);
    dc.DrawLine(sp2, sp3);
    dc.DrawLine(sp3, sp1);
}

static const float HOUSE_X = 320.0;
static const float HOUSE_Y = 320.0;
static const float HOUSE_BASE_Z = 0.0;

void Render::DrawHouse(wxPaintDC &dc)
{
    const float halfS = 8.0f;
    const float h = 6.0f;
    const float roofH = 6.0f;

    // 底面 4点
    Vec3 bbl(HOUSE_X - halfS, HOUSE_Y - halfS, HOUSE_BASE_Z);
    Vec3 bbr(HOUSE_X + halfS, HOUSE_Y - halfS, HOUSE_BASE_Z);
    Vec3 fbr(HOUSE_X + halfS, HOUSE_Y + halfS, HOUSE_BASE_Z);
    Vec3 fbl(HOUSE_X - halfS, HOUSE_Y + halfS, HOUSE_BASE_Z);

    // 顶面 4点
    Vec3 tpl(HOUSE_X - halfS, HOUSE_Y - halfS, HOUSE_BASE_Z + h);
    Vec3 tpr(HOUSE_X + halfS, HOUSE_Y - halfS, HOUSE_BASE_Z + h);
    Vec3 tfr(HOUSE_X + halfS, HOUSE_Y + halfS, HOUSE_BASE_Z + h);
    Vec3 tfl(HOUSE_X - halfS, HOUSE_Y + halfS, HOUSE_BASE_Z + h);

    // 尖顶中心点
    Vec3 roofTop(HOUSE_X, HOUSE_Y, HOUSE_BASE_Z + h + roofH);

    // -------- 完整立方体6个面 --------
    // 前墙
    DrawQuad(dc, fbl, fbr, tfr, tfl);
    // 后墙
    DrawQuad(dc, bbl, bbr, tpr, tpl);
    // 左墙
    DrawQuad(dc, bbl, fbl, tfl, tpl);
    // 右墙
    DrawQuad(dc, bbr, fbr, tfr, tpr);
    // 底面
    DrawQuad(dc, bbl, bbr, fbr, fbl);
    // 顶面
    DrawQuad(dc, tpl, tpr, tfr, tfl);

    // -------- 完整四棱锥尖顶（4个三角屋顶） --------
    DrawTriangle(dc, roofTop, tpl, tpr);
    DrawTriangle(dc, roofTop, tpr, tfr);
    DrawTriangle(dc, roofTop, tfr, tfl);
    DrawTriangle(dc, roofTop, tfl, tpl);

    // DrawDoor(dc, false);
    // DrawWindows(dc, false);
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