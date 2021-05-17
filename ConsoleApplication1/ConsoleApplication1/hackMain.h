#pragma once
#include <iostream>
#include "windows.h"
#include "hackMain.h"
#include <thread>
#include "mFuncs.h"
#include <vector>

namespace Main
{
    bool db1 = false;
    bool db2 = false;
    bool db3 = false;
    bool db4 = false;
    bool db5 = false;

    void CalcAngle(float* src, float* dst, float* angles)
    {
        double delta[3] = { (src[0] - dst[0]), (src[1] - dst[1]), (src[2] - dst[2]) };
        double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
        angles[0] = (float)(asinf(delta[2] / hyp) * 57.295779513082f);
        angles[1] = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
        angles[2] = 0.0f;
        if (delta[0] >= 0.0) { angles[1] += 180.0f; }
    }

    void tBot()
    {
        std::cout << "parsed" << std::endl;
        if (globalvars::triggerbot)
        {
            while (globalvars::triggerbot)
            {
                DWORD localPlr = memory::RPM<DWORD>(globals::moduleBase + hazedumper::signatures::dwLocalPlayer);
                int localTeam = memory::RPM<int>(localPlr + hazedumper::netvars::m_iTeamNum);
                int chID = memory::RPM<int>(localPlr + hazedumper::netvars::m_iCrosshairId);
                if (chID != 0 && chID < 64)
                {
                    //int chID = RPM
                    DWORD eList = memory::RPM<DWORD>(globals::moduleBase + hazedumper::signatures::dwEntityList + ((chID - 1) * 0x10));
                    int eHP = memory::RPM<int>(eList + hazedumper::netvars::m_iHealth);
                    int eTeam = memory::RPM<int>(eList + hazedumper::netvars::m_iTeamNum);
                    if (localTeam != eTeam && eHP > 0)
                    {
                        std::cout << eTeam << std::endl;
                        std::cout << localTeam << std::endl;
                        Sleep(250);
                        memory::WPM<int>(globals::moduleBase + hazedumper::signatures::dwForceAttack, 5);
                        Sleep(10);
                        memory::WPM<int>(globals::moduleBase + hazedumper::signatures::dwForceAttack, 4);
                    }

                }
            }
        }
    }
    void gdiEsp()
    {
        HBRUSH brush = CreateSolidBrush(0x0000FF00);//0x000000FF
        HBRUSH tbrush = CreateSolidBrush(0x000000FF);//0x0000FF00
        while (globalvars::gdiEsp)
        {
            int teamNum = memory::RPM<int>(memory::getLocalPlayer() + hazedumper::netvars::m_iTeamNum);
            video::mat4x4 viewMatrix = memory::RPM<video::mat4x4>((globals::moduleBase)+hazedumper::signatures::dwViewMatrix);
            uintptr_t localPlayer = memory::getLocalPlayer();
            if (globalvars::gdiEsp)
            {
                for (size_t i = 0; i < 64; i++)
                {
                    uintptr_t enemyI = memory::RPM<uintptr_t>(globals::moduleBase + hazedumper::signatures::dwEntityList + i * 0x10);

                    // hp checks here n all

                   // if (enemyI == NULL)
                   //     continue;

                    bool bDormant = memory::RPM<bool>(enemyI + hazedumper::signatures::m_bDormant);

                    int enemyHP = memory::RPM<int>(enemyI + hazedumper::netvars::m_iHealth);

                    if (enemyHP <= 0)
                        continue;
                    int eTeamNum = memory::RPM<int>(localPlayer + hazedumper::netvars::m_iTeamNum);

                    if (teamNum != eTeamNum && enemyI != localPlayer)
                    {
                        globals::vec3 pos = memory::RPM<globals::vec3>(enemyI + hazedumper::netvars::m_vecOrigin);
                        globals::vec3 head;
                        //globals::vec3 ret = video::WorldToScreen(ePos,viewMatrix);
                        globals::vec3 screenpos = video::WorldToScreen(head, viewMatrix);
                        globals::vec3 screenhead = video::WorldToScreen(pos, viewMatrix);
                        head.x = pos.x;
                        head.y = pos.y;
                        head.z = pos.z + 75.f;

                        float height = screenhead.y - screenpos.y;
                        float width = height / 2.4f;
                        if (screenpos.z >= 0.01f)
                        {
                            video::DrawBorderBox(screenpos.x - (width / 2), screenpos.y, width, height, 2, brush);
                            int chp = (width / 100) * enemyHP;
                            video::DrawFilledRect((screenpos.x - (width / 2)) - 10, screenpos.y, (width / 9), (chp * 2.5), brush);
                        }
                    }
                    else
                    {
                        if (enemyI != localPlayer)
                        {
                            globals::vec3 pos = memory::RPM<globals::vec3>(enemyI + hazedumper::netvars::m_vecOrigin);
                            globals::vec3 head;
                            //globals::vec3 ret = video::WorldToScreen(ePos,viewMatrix);
                            head.x = pos.x;
                            head.y = pos.y;
                            head.z = pos.z + 75.f;

                            globals::vec3 screenpos = video::WorldToScreen(head, viewMatrix);
                            globals::vec3 screenhead = video::WorldToScreen(pos, viewMatrix);
                            float height = screenhead.y - screenpos.y;
                            float width = height / 2.4f;

                            if (screenpos.z >= 0.01f)
                            {
                                video::DrawBorderBox(screenpos.x - (width / 2), screenpos.y, width, height, 1, tbrush);

                                int chp = (width / 100) * enemyHP;
                                video::DrawFilledRect(screenpos.x - (width / 2), screenpos.y, (width / 9), (chp * 2.5), brush);
                            }                                                                            
                        }
                    }
                }
            }
        }
    }
    void glowEsp()
    {
        while (globalvars::glowEsp)
        {
            uintptr_t glowManager = memory::RPM<uintptr_t>(globals::moduleBase + hazedumper::signatures::dwGlowObjectManager);
            int teamNum = memory::RPM<int>(memory::getLocalPlayer() + hazedumper::netvars::m_iTeamNum);
            if (globalvars::glowEsp)
            {
                for (size_t i = 0; i <= 64; i++)
                {
                    // get bomb carrier in the and loop through ent list
                    // Also 
                    uintptr_t enemyI = memory::RPM<uintptr_t>(globals::moduleBase + hazedumper::signatures::dwEntityList + i * 0x10);
                    if (enemyI == NULL)
                        continue;

                    int glowindex = memory::RPM<int>(enemyI + hazedumper::netvars::m_iGlowIndex);

                    int eteam = memory::RPM<int>(enemyI + hazedumper::netvars::m_iTeamNum);

                    bool bDormant = memory::RPM<bool>(enemyI + hazedumper::signatures::m_bDormant);

                    if (bDormant == true)
                        continue;

                    int enemyHP = memory::RPM<int>(enemyI + hazedumper::netvars::m_iHealth);

                    if (enemyHP <= 0)
                        continue;

                    if (teamNum == eteam)
                    {
                        memory::WPM<float>(glowManager + glowindex * 0x38 + 0x4, 0);
                        memory::WPM<float>(glowManager + glowindex * 0x38 + 0x8, 255);
                        memory::WPM<float>(glowManager + glowindex * 0x38 + 0xC, 0);
                        memory::WPM<float>(glowManager + glowindex * 0x38 + 0x10, 255);
                        memory::WPM<bool>(glowManager + glowindex * 0x38 + 0x24, true);
                        memory::WPM<bool>(glowManager + glowindex * 0x38 + 0x25, false);
                    }
                    else
                    {
                        memory::WPM<float>(glowManager + glowindex * 0x38 + 0x4, 255);
                        memory::WPM<float>(glowManager + glowindex * 0x38 + 0x8, 0);
                        memory::WPM<float>(glowManager + glowindex * 0x38 + 0xC, 0);
                        memory::WPM<float>(glowManager + glowindex * 0x38 + 0x10, 255);
                        memory::WPM<bool>(glowManager + glowindex * 0x38 + 0x24, true);
                        memory::WPM<bool>(glowManager + glowindex * 0x38 + 0x25, false);
                    }
                }

            }
        }
    }
    void boneESP()
    {
        HBRUSH brush = CreateSolidBrush(0x0000FF00);//0x000000FF
        HBRUSH tbrush = CreateSolidBrush(0x000000FF);//0x0000FF00
        while (globalvars::boneEsp)
        {
            int teamNum = memory::RPM<int>(memory::getLocalPlayer() + hazedumper::netvars::m_iTeamNum);
            video::mat4x4 viewMatrix = memory::RPM<video::mat4x4>((globals::moduleBase)+hazedumper::signatures::dwViewMatrix);
            uintptr_t localPlayer = memory::getLocalPlayer();
            for (size_t i = 0; i < 64; i++)
            {
                // modeltype
                char* operatorN[128];
                uintptr_t enemyI = memory::RPM<uintptr_t>(globals::moduleBase + hazedumper::signatures::dwEntityList + i * 0x10);
                DWORD boneBase = memory::RPM<DWORD>(enemyI + hazedumper::netvars::m_dwBoneMatrix);
               // char* operatorName = memory::RPM<char*>(*(int*)(enemyI + 0x6C) + 0x04);
               // strcpy(*operatorN, operatorName);
               // std::string name(*operatorN);// now make into string

                const char* Separatist = "tm_separatist";
                // main
                video::DrawLine(500,200,510,600,10);

              

            }
        }
    }
    void aimBot()
    {
        while (globalvars::aimAssit)
        {
            int teamNum = memory::RPM<int>(memory::getLocalPlayer() + hazedumper::netvars::m_iTeamNum);
            video::mat4x4 viewMatrix = memory::RPM<video::mat4x4>((globals::moduleBase)+hazedumper::signatures::dwViewMatrix);
            uintptr_t localPlayer = memory::getLocalPlayer();
            globals::vec3 enemyCoordinate;
            globals::vec3 localCoordinate;
            globals::vec3 totalCoordinate;

            for (size_t i = 0; i < 64; i++)
            {
                uintptr_t enemyI = memory::RPM<uintptr_t>(globals::moduleBase + hazedumper::signatures::dwEntityList + i * 0x10);
                localCoordinate = video::GetBonePosition(localPlayer,6);
                enemyCoordinate = video::GetBonePosition(enemyI, 6);

                totalCoordinate.x = enemyCoordinate.x - localCoordinate.x;
                totalCoordinate.y = enemyCoordinate.y - localCoordinate.y;
                totalCoordinate.z = enemyCoordinate.z - localCoordinate.z;

                float add = sqrt(pow(totalCoordinate.x, 2) + pow(totalCoordinate.y, 2));

                float posA = totalCoordinate.x;
                float posB = totalCoordinate.y;
                float posC = add;

                float yaw = atan(posB / posA);
                float pitch = -atan(0 / posC);
                std::cout << "Pitch: " << pitch << std::endl;
                std::cout << "Yaw: " << yaw << std::endl;
                system("cls");

            }


        }
    }
    
    void hackMain()
	{
        while (!GetAsyncKeyState(VK_END))
        {
            if (GetAsyncKeyState(VK_F9))
            {
                if (globalvars::glowEsp)
                    globalvars::glowEsp = false;
                else
                    globalvars::glowEsp = true;
            }
            if (GetAsyncKeyState(VK_F10))
            {
                if (globalvars::gdiEsp)
                    globalvars::gdiEsp = false;
                else
                    globalvars::gdiEsp = true;
            }
            if (globalvars::aimAssit)
            {
                
            }
          
            if (db1 == false)
            {
                CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)tBot, NULL,NULL,NULL);
                db1 = true;
            }
            if (db2 == false)
            {
                CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)gdiEsp, NULL,NULL,NULL);
                db2 = true;
            }
            if (db3 == false)
            {
                CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)glowEsp, NULL,NULL,NULL);
                db3 = true;
            }
            if (db4 == false)
            {
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)boneESP, NULL, NULL, NULL);
                db4 = true;
            }
            if (db5 == false)
            {
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)aimBot , NULL, NULL, NULL);
                db5 = true;
            }

            
        }
	}
}