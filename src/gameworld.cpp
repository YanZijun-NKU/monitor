#include "gameworld.h"
#include <algorithm>
#include <initializer_list>
GameWorld world;

void GameWorld::Update(float dt)
{
    float PosXAxis = -sinf(player.yaw);
    float PosYAxis = cosf(player.yaw);

    float fx = player.movespeed * dt * PosXAxis;
    float fy = player.movespeed * dt * PosYAxis;

    float oldX = player.x;
    float oldY = player.y;

    float newX = oldX;
    float newY = oldY;

    if (input.w)
    {
        newX += fx;
        newY += fy;
    }
    if (input.a)
    {
        newX -= fy;
        newY += fx;
    }
    if (input.s)
    {
        newX -= fx;
        newY -= fy;
    }
    if (input.d)
    {
        newX += fy;
        newY -= fx;
    }

    bool blockMove = false;
    const float faceThick = 0.25f; // 只往墙面正反加厚，侧面不加

    for (const auto &col : m_collisions)
    {
        if (col.type == TRI)
            continue;

        float x1 = col.sp1.x, x2 = col.sp2.x, x3 = col.sp3.x, x4 = col.sp4.x;
        float y1 = col.sp1.y, y2 = col.sp2.y, y3 = col.sp3.y, y4 = col.sp4.y;
        float z1 = col.sp1.z, z2 = col.sp2.z, z3 = col.sp3.z, z4 = col.sp4.z;

        float wxMin = std::min(std::min(x1, x2), std::min(x3, x4));
        float wxMax = std::max(std::max(x1, x2), std::max(x3, x4));
        float wyMin = std::min(std::min(y1, y2), std::min(y3, y4));
        float wyMax = std::max(std::max(y1, y2), std::max(y3, y4));
        float wzMin = std::min(std::min(z1, z2), std::min(z3, z4));
        float wzMax = std::max(std::max(z1, z2), std::max(z3, z4));

        bool inZ = (player.z >= wzMin) && (player.z <= wzMax);
        if (!inZ)
            continue;

        // 前后墙：Y向薄片 → 只加厚Y，X不动！
        if (wyMax - wyMin < 0.05f)
        {
            // 法线方向Y加厚，X完全不变
            float bMinY = wyMin - faceThick;
            float bMaxY = wyMin + faceThick;

            // X用原始真实墙范围
            bool inX = (newX >= wxMin) && (newX <= wxMax);
            bool inY = (newY >= bMinY) && (newY <= bMaxY);

            if (inX && inY)
            {
                blockMove = true;
                break;
            }
        }

        // 左右墙：X向薄片 → 只加厚X，Y不动！
        if (wxMax - wxMin < 0.05f)
        {
            // 法线方向X加厚，Y完全不变
            float bMinX = wxMin - faceThick;
            float bMaxX = wxMin + faceThick;

            // Y用原始真实墙范围
            bool inY = (newY >= wyMin) && (newY <= wyMax);
            bool inX = (newX >= bMinX) && (newX <= bMaxX);

            if (inX && inY)
            {
                blockMove = true;
                break;
            }
        }
    }

    if (!blockMove)
    {
        player.x = newX;
        player.y = newY;
    }

    const float gravity = 9.8;
    const float jumpPower = 5.0;
    if (input.space && player.z <= 1.0)
    {
        player.vz = jumpPower;
    }
    player.vz -= gravity * dt;
    player.z += player.vz * dt;
    if (player.z <= 1.0)
    {
        player.z = 1.0;
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