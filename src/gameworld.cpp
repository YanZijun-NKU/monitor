#include "gameworld.h"

GameWorld world;

void GameWorld::Update(float dt)
{
    // 先确定前进方向，xyz是绝对坐标系
    float PosXAxis = -sinf(player.yaw);
    float PosYAxis = cosf(player.yaw);

    float fx = player.movespeed * dt * PosXAxis;
    float fy = player.movespeed * dt * PosYAxis;

    if (input.w)
    {
        player.x += fx;
        player.y += fy;
    }
    if (input.a)
    {
        player.x -= fy;
        player.y += fx;
    }
    if (input.s)
    {
        player.x -= fx;
        player.y -= fy;
    }
    if (input.d)
    {
        player.x += fy;
        player.y -= fx;
    }

    const float gravity = 9.8;
    const float jumpPower = 4.5;
    if (input.space && player.z <= 0)
    {
        player.vz = jumpPower;
    }
    player.vz -= gravity * dt;
    player.z += player.vz * dt;
    if (player.z < 0)
    {
        player.z = 0;
        player.vz = 0;
    }

    if (input.e)
    {
        if (!m_ePressed)
        {
            m_ePressed = true;
            house.ToggleDoor();
        }
    }
    else
    {
        m_ePressed = false;
    }

    if (input.q)
    {
        if (!m_qPressed)
        {
            m_qPressed = true;
            house.ToggleWindows();
        }
    }
    else
    {
        m_qPressed = false;
    }
}