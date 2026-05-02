#pragma once
#include <cmath>
#include <vector>
#include <cstring>
#include <wx/wx.h>
// 这部分用于存储状态设置，类似于.ini文件，是纯文本，不负责图形化
// c++特性：struct和public class没有区别，因此struct 内部也可以写函数。c不支持
struct Player
{
    float x = 300.0, y = 300.0, z = 300.0;
    float yaw = 0;
    float pitch = 0;
    float movespeed = 15.0;
    float rotspeed = 0.01;
    float vz = 0;
};
struct House
{
    bool Dooropen = false;
    bool Windowsopen = false;
    float cx = 320.0;
    float cy = 320.0;
    void ToggleDoor()
    {
        Dooropen = !Dooropen;
    }
    void ToggleWindows()
    {
        Windowsopen = !Windowsopen;
    }
};
struct GameInput
{
    bool w = 0, a = 0, s = 0, d = 0;
    bool space = 0;
    bool e = 0, q = 0;
    void SetKeyDown(int key)
    {
        switch (key)
        {
        case 'A':
        case 'a':
            a = 1;
            break;
        case 'W':
        case 'w':
            w = 1;
            break;
        case 'S':
        case 's':
            s = 1;
            break;
        case 'D':
        case 'd':
            d = 1;
            break;
        case WXK_SPACE:
            space = 1;
            break;
        case 'E':
        case 'e':
            e = 1;
            break;
        case 'Q':
        case 'q':
            q = 1;
            break;
        }
    }
    void SetKeyUp(int key)
    {
        switch (key)
        {
        case 'A':
        case 'a':
            a = 0;
            break;
        case 'W':
        case 'w':
            w = 0;
            break;
        case 'S':
        case 's':
            s = 0;
            break;
        case 'D':
        case 'd':
            d = 0;
            break;
        case WXK_SPACE:
            space = 0;
            break;
        case 'E':
        case 'e':
            e = 0;
            break;
        case 'Q':
        case 'q':
            q = 0;
            break;
        }
    }
};
class GameWorld
{
public:
    float worldrange = 700.0;
    Player player;
    House house;
    GameInput input;
    void Update(float dt);

private:
    bool m_ePressed = false;
    bool m_qPressed = false; // 长按锁
    bool SpacePressed = false;
};

extern GameWorld world;