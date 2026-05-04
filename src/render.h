#pragma once
#include <wx/wx.h>
#include <vector>
struct Vec3
{
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float a, float b, float c) : x(a), y(b), z(c) {}
};

class Render
{
public:
    Render(int w, int h);
    void SetCamera(Vec3 c, float y, float p);
    wxPoint WorldToScr(Vec3 worldPt);
    void DrawQuad(wxPaintDC &dc, Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4);
    void DrawTriangle(wxPaintDC &dc, Vec3 p1, Vec3 p2, Vec3 p3);
    void DrawHouse(wxPaintDC &dc);
    void DrawWindows(wxPaintDC &dc, bool isOpen);
    void DrawDoor(wxPaintDC &dc, bool isOpen);
    void Resize(int w, int h);

    void ZoomIn();  // 放大
    void ZoomOut(); // 缩小

private:
    const float m_minfov = 0.52f;
    const float m_maxfov = 2.09f;
    float m_fov;
    float m_near;
    int m_winH, m_winW;
    Vec3 Campos;
    float yaw, pitch;
};
