#pragma once
#include <iostream>
#include "windows.h"
#include "hackMain.h"
#include "mFuncs.h"
#include <vector>

namespace position
{


    struct mat3x4
    {
        float* operator[ ](int index) {
            return c[index];
        }
        float c[3][4];
    };


    mat3x4 GetBoneMatrix(DWORD entity, int boneCount)
    {
        DWORD BoneMatrixPtr = memory::RPM<DWORD>(entity + hazedumper::netvars::m_dwBoneMatrix);
        return memory::RPM<mat3x4>(BoneMatrixPtr + (0x30 * boneCount));
    }

    video::mat4x4 getViewMatrix(DWORD matrixAddress)
    {
        return memory::RPM<video::mat4x4>(matrixAddress);
    }

    globals::vec3 GetBonePosition(DWORD entity, int BoneIndex)
    {
        DWORD BoneMatrixPtr = memory::RPM<DWORD>(entity + hazedumper::netvars::m_dwBoneMatrix);
        mat3x4 BoneMatrix = memory::RPM<mat3x4>(BoneMatrixPtr + (0x30 * BoneIndex));
        globals::vec3 i; // buffer
        i.x = BoneMatrix.c[0][3];
        i.y = BoneMatrix.c[1][3];
        i.z = BoneMatrix.c[2][3];
        
        return i;
    }
}

namespace Main
{
    void tBot()
    {
    }

	void hackMain()
	{
        HBRUSH brush = CreateSolidBrush(0x0000FF00);//0x000000FF
        HBRUSH tbrush = CreateSolidBrush(0x000000FF);//0x0000FF00
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
            uintptr_t glowManager = memory::RPM<uintptr_t>(globals::moduleBase + hazedumper::signatures::dwGlowObjectManager);
            int teamNum = memory::RPM<int>(memory::getLocalPlayer() + hazedumper::netvars::m_iTeamNum);
            video::mat4x4 viewMatrix = memory::RPM<video::mat4x4>((globals::moduleBase) + hazedumper::signatures::dwViewMatrix);
            uintptr_t localPlayer = memory::getLocalPlayer();

            if (globalvars::aimAssit)
            {
                
            }
           
            


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

                    if (globalvars::triggerbot == true)
                    {
                        int crosshair = memory::RPM<int>(localPlayer + hazedumper::netvars::m_iCrosshairId);
                        if (crosshair != 0 && crosshair < 64)
                        {
                            DWORD enT = memory::RPM<DWORD>(enemyI + crosshair);
                            int enTTeam = memory::RPM<int>(enemyI + hazedumper::netvars::m_iTeamNum);
                            if (enTTeam != eTeamNum && enemyHP > 0)
                            {
                                // if they're not on our team
                                memory::WPM<int>(globals::moduleBase + hazedumper::signatures::dwForceAttack, 5);
                                Sleep(1);
                                memory::WPM<int>(globals::moduleBase + hazedumper::signatures::dwForceAttack, 4);
                            }
                        }
                    }

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
                            video::DrawBorderBox(screenpos.x - (width / 2), screenpos.y, width, height, 1, brush);
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
                            }
                        }
                    }
                }
            }

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
}