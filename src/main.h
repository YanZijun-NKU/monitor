#pragma once
#include <wx/wx.h>
#include "map3D.h"
class MainFrame : public wxFrame
{
public:
    MainFrame(); // 构造函数，没有返回类型
    Map3D *GetMap3D()
    {
        return map;
    }
    //~MainFrame();
private:
    Map3D *map;
    Map2D *mp2d;
};
