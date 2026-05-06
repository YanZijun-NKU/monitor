#include "render.h"
#include "gameworld.h"

Render::Render(int w, int h)
{
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
    float dx1 = worldPt.x - Campos.x;
    float dy1 = worldPt.y - Campos.y;
    float dz1 = worldPt.z - Campos.z;

    float dx2 = dx1 * cosf(-yaw) - dy1 * sinf(-yaw);
    float dy2 = dy1 * cosf(-yaw) + dx1 * sinf(-yaw);
    float dz2 = dz1;

    float dx = dx2;
    float dy = dy2 * cosf(-pitch) - dz2 * sinf(-pitch);
    float dz = dy2 * sinf(-pitch) + dz2 * cosf(-pitch);

    if (dy < m_near)
        dy = m_near;
    float scale = 1.0f / tanf(m_fov * 0.5f);
    float px = scale * dx / dy;
    float py = scale * dz / dy;

    int sx = (int)(m_winW * 0.5f + px * m_winW * 0.5f);
    int sy = (int)(m_winH * 0.5f - py * m_winH * 0.5f);
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
    world.m_collisions.clear();
    const float halfS = 8.0f;
    const float h = 6.0f;
    const float roofH = 6.0f;

    Vec3 bbl(HOUSE_X - halfS, HOUSE_Y - halfS, HOUSE_BASE_Z);
    Vec3 bbr(HOUSE_X + halfS, HOUSE_Y - halfS, HOUSE_BASE_Z);
    Vec3 fbr(HOUSE_X + halfS, HOUSE_Y + halfS, HOUSE_BASE_Z);
    Vec3 fbl(HOUSE_X - halfS, HOUSE_Y + halfS, HOUSE_BASE_Z);

    Vec3 tpl(HOUSE_X - halfS, HOUSE_Y - halfS, HOUSE_BASE_Z + h);
    Vec3 tpr(HOUSE_X + halfS, HOUSE_Y - halfS, HOUSE_BASE_Z + h);
    Vec3 tfr(HOUSE_X + halfS, HOUSE_Y + halfS, HOUSE_BASE_Z + h);
    Vec3 tfl(HOUSE_X - halfS, HOUSE_Y + halfS, HOUSE_BASE_Z + h);

    Vec3 roofTop(HOUSE_X, HOUSE_Y, HOUSE_BASE_Z + h + roofH);

    DrawQuad(dc, fbl, fbr, tfr, tfl);
    Collision cw1;
    cw1.sp1 = fbl;
    cw1.sp2 = fbr;
    cw1.sp3 = tfr;
    cw1.sp4 = tfl;
    cw1.type = RECT;
    world.m_collisions.push_back(cw1);

    // 前墙：只保留原有绘制，不整体加碰撞
    DrawQuad(dc, bbl, bbr, tpr, tpl);

    float frontY = HOUSE_Y - halfS;

    {
        Collision col;
        col.sp1 = Vec3(HOUSE_X - 8, frontY, 0.0f);
        col.sp2 = Vec3(HOUSE_X - 2, frontY, 0.0f);
        col.sp3 = Vec3(HOUSE_X - 8, frontY, 6);
        col.sp4 = Vec3(HOUSE_X - 2, frontY, 6);
        col.type = RECT;
        world.m_collisions.push_back(col);
    }

    // 门左上侧碰撞
    {
        Collision col;
        col.sp1 = Vec3(HOUSE_X - 2, frontY, 3.2f);
        col.sp2 = Vec3(HOUSE_X + 2, frontY, 3.2f);
        col.sp3 = Vec3(HOUSE_X - 2, frontY, 6.0f);
        col.sp4 = Vec3(HOUSE_X + 2, frontY, 6.0f);
        col.type = RECT;
        world.m_collisions.push_back(col);
    }

    // // 门与窗户中间墙体碰撞
    {
        Collision col;
        col.sp1 = Vec3(HOUSE_X + 2, frontY, 0);
        col.sp2 = Vec3(HOUSE_X + 4, frontY, 0);
        col.sp3 = Vec3(HOUSE_X + 4, frontY, 6.0f);
        col.sp4 = Vec3(HOUSE_X + 2, frontY, 6.0f);
        col.type = RECT;
        world.m_collisions.push_back(col);
    }

    // // 窗户上方碰撞
    {
        Collision col;
        col.sp1 = Vec3(HOUSE_X + 4, frontY, 3.5f);
        col.sp2 = Vec3(HOUSE_X + 7, frontY, 3.5f);
        col.sp3 = Vec3(HOUSE_X + 7, frontY, 6.0f);
        col.sp4 = Vec3(HOUSE_X + 4, frontY, 6.0f);
        col.type = RECT;
        world.m_collisions.push_back(col);
    }

    // // 窗户右侧碰撞
    {
        Collision col;
        col.sp1 = Vec3(HOUSE_X + 7, frontY, 0);
        col.sp2 = Vec3(HOUSE_X + 8, frontY, 0);
        col.sp3 = Vec3(HOUSE_X + 8, frontY, 6.0f);
        col.sp4 = Vec3(HOUSE_X + 7, frontY, 6.0f);
        col.type = RECT;
        world.m_collisions.push_back(col);
    }

    {
        Collision col;
        col.sp1 = Vec3(HOUSE_X + 4, frontY, 0);
        col.sp2 = Vec3(HOUSE_X + 7, frontY, 0);
        col.sp3 = Vec3(HOUSE_X + 4, frontY, 2.0f);
        col.sp4 = Vec3(HOUSE_X + 7, frontY, 2.0f);
        col.type = RECT;
        world.m_collisions.push_back(col);
    }

    DrawQuad(dc, bbl, fbl, tfl, tpl);
    Collision cw3;
    cw3.sp1 = bbl;
    cw3.sp2 = fbl;
    cw3.sp3 = tfl;
    cw3.sp4 = tpl;
    cw3.type = RECT;
    world.m_collisions.push_back(cw3);

    DrawQuad(dc, bbr, fbr, tfr, tpr);
    Collision cw4;
    cw4.sp1 = bbr;
    cw4.sp2 = fbr;
    cw4.sp3 = tfr;
    cw4.sp4 = tpr;
    cw4.type = RECT;
    world.m_collisions.push_back(cw4);

    DrawQuad(dc, bbl, bbr, fbr, fbl);
    // Collision cw5;
    // cw5.sp1 = bbl;
    // cw5.sp2 = bbr;
    // cw5.sp3 = fbr;
    // cw5.sp4 = fbl;
    // cw5.type = RECT;
    // world.m_collisions.push_back(cw5);

    DrawQuad(dc, tpl, tpr, tfr, tfl);
    // Collision cw6;
    // cw6.sp1 = tpl;
    // cw6.sp2 = tpr;
    // cw6.sp3 = tfr;
    // cw6.sp4 = tfl;
    // cw6.type = RECT;
    // world.m_collisions.push_back(cw6);

    DrawTriangle(dc, roofTop, tpl, tpr);
    Collision cw7;
    cw7.sp1 = roofTop;
    cw7.sp2 = tpl;
    cw7.sp3 = tpr;
    cw7.type = TRI;
    world.m_collisions.push_back(cw7);

    DrawTriangle(dc, roofTop, tpr, tfr);
    Collision cw8;
    cw8.sp1 = roofTop;
    cw8.sp2 = tpr;
    cw8.sp3 = tfr;
    cw8.type = TRI;
    world.m_collisions.push_back(cw8);

    DrawTriangle(dc, roofTop, tfr, tfl);
    Collision cw9;
    cw9.sp1 = roofTop;
    cw9.sp2 = tfr;
    cw9.sp3 = tfl;
    cw9.type = TRI;
    world.m_collisions.push_back(cw9);

    DrawTriangle(dc, roofTop, tfl, tpl);
    Collision cw10;
    cw10.sp1 = roofTop;
    cw10.sp2 = tfl;
    cw10.sp3 = tpl;
    cw10.type = TRI;
    world.m_collisions.push_back(cw10);
}

void Render::DrawDoor(wxPaintDC &dc, bool isOpen)
{
    if (!isOpen)
    {
        Vec3 d1(HOUSE_X - 2, HOUSE_Y - 8, HOUSE_BASE_Z);
        Vec3 d2(HOUSE_X + 2, HOUSE_Y - 8, HOUSE_BASE_Z);
        Vec3 d3(HOUSE_X + 2, HOUSE_Y - 8, HOUSE_BASE_Z + 3.2f);
        Vec3 d4(HOUSE_X - 2, HOUSE_Y - 8, HOUSE_BASE_Z + 3.2f);
        Collision doorCol;
        doorCol.sp1 = d1;
        doorCol.sp2 = d2;
        doorCol.sp3 = d3;
        doorCol.sp4 = d4;
        doorCol.type = RECT;
        world.m_collisions.push_back(doorCol);
        DrawQuad(dc, d1, d2, d3, d4);
    }
    else
    {
        Vec3 d1(HOUSE_X - 2, HOUSE_Y - 8, HOUSE_BASE_Z);
        Vec3 d2(HOUSE_X - 2, HOUSE_Y - 4, HOUSE_BASE_Z);
        Vec3 d3(HOUSE_X - 2, HOUSE_Y - 4, HOUSE_BASE_Z + 3.2f);
        Vec3 d4(HOUSE_X - 2, HOUSE_Y - 8, HOUSE_BASE_Z + 3.2f);
        DrawQuad(dc, d1, d2, d3, d4);
    }
}

void Render::DrawWindows(wxPaintDC &dc, bool isOpen)
{
    if (!isOpen)
    {
        Vec3 w1(HOUSE_X + 4, HOUSE_Y - 8, HOUSE_BASE_Z + 2);
        Vec3 w2(HOUSE_X + 7, HOUSE_Y - 8, HOUSE_BASE_Z + 2);
        Vec3 w3(HOUSE_X + 7, HOUSE_Y - 8, HOUSE_BASE_Z + 3.5f);
        Vec3 w4(HOUSE_X + 4, HOUSE_Y - 8, HOUSE_BASE_Z + 3.5f);
        DrawQuad(dc, w1, w2, w3, w4);
        Collision winCol;
        winCol.sp1 = w1;
        winCol.sp2 = w2;
        winCol.sp3 = w3;
        winCol.sp4 = w4;
        winCol.type = RECT;
        world.m_collisions.push_back(winCol);
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