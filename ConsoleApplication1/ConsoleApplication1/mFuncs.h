#pragma once
#include "windows.h"
#include "csgo.hpp"
#include <Psapi.h>
#include <TlHelp32.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

namespace globals
{
	struct lGlowStructEnemy
	{
		float red = 1.0f;
		float green = 0.0f;
		float blue = 0.0f;
		float alpha = 1.0f;
		uint8_t padding[8];
		float unknown = 1.0f;
		uint8_t padding2[4];
		BYTE renderOccluded = true;
		BYTE renderUnoccluded = false;
		BYTE fullBloom = false;

	}glowStructEnemy;
	
	struct lGlowStructTeam
	{
		float red = 0.0f;
		float green = 1.0f;
		float blue = 0.0f;
		float alpha = 1.0f;
		uint8_t padding[8];
		float unknown = 1.0f;
		uint8_t padding2[4];
		BYTE renderOccluded = true;
		BYTE renderUnoccluded = false;
		BYTE fullBloom = false;

	}glowStructTeam;


	struct vec3
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};
	//tool
	DWORD getModule(LPCSTR ModuleName, UINT32 dwPID)
	{
		HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(mEntry);
		Module32First(hModule, &mEntry);
		do
			if (!strcmp(mEntry.szModule, (LPSTR)ModuleName))
			{
				CloseHandle(hModule);
				return (DWORD)mEntry.modBaseAddr;
			}
		while (Module32Next(hModule, &mEntry));
		CloseHandle(hModule);
		return -1;
	}
	// main vars
	
	HWND counterstrikeHandle; // assigned in main
	DWORD moduleBase;
	DWORD* localPlayer;
	HANDLE hproc;
	HDC hdc;


	int screenX = 1920;
	int screenY = 1080;
	

	bool initModBase()
	{
		 localPlayer = (DWORD*)*(DWORD*)(moduleBase + hazedumper::signatures::dwLocalPlayer);
		 moduleBase = (DWORD)GetModuleHandle("Client.dll");
		 if (moduleBase != NULL)
			 return true;
		 return false;
	}

}

namespace globalvars
{
	bool triggerbot = false;
	bool boneEsp = true;
	bool glowEsp = true;
	bool gdiEsp = false;
	bool aimAssit = true;
}

namespace video
{
	float slope(float x1, float y1, float x2, float y2)
	{
		return (y2 - y1) / (x2 - x1);
	}


	void DrawFilledRect(int x, int y, int w, int h, HBRUSH brush)
	{
		RECT rect = { x, y, x + w, y + h };
		FillRect(globals::hdc , &rect, brush);
	}

	void DrawLine(float x1, float y1, float x2, float y2, float thickness)
	{
		float s = slope(x1,y1,x2,y2);
		float distBetween = x1 - x2;
		HBRUSH brush = CreateSolidBrush(0x00FFFFFF);
		for (size_t i = x1; i < x2; i++)
		{
			DrawFilledRect(i,(y1 - s), thickness,thickness,brush);
		}
	}

	void DrawBorderBox(int x, int y, int w, int h, int thickness, HBRUSH brush)
	{
		DrawFilledRect(x, y, w, thickness, brush); //Top horiz line
		DrawFilledRect(x, y, thickness, h, brush); //Left vertical line
		DrawFilledRect((x + w), y, thickness, h, brush); //right vertical line
		DrawFilledRect(x, y + h, w + thickness, thickness, brush); //bottom horiz line
	}

	struct mat4x4
	{
		float* operator[ ](int index) {
			return matrix[index];
		}
		float matrix[4][4];
	};

	globals::vec3 WorldToScreen(globals::vec3 pos, mat4x4 matrix) 
	{
		float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
		float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

		float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];
		
		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = globals::screenX * .5f;
		float y = globals::screenY * .5f;

		x += 0.5f * _x * globals::screenX + 0.5f;
		y -= 0.5f * _y * globals::screenY + 0.5f;

		return { x,y,w };
	}


}

namespace memory
{
	template<typename T>
	T RPM(uintptr_t baseAddress, uintptr_t offSet = 0)
	{
		T lpBuf;
		ReadProcessMemory(globals::hproc,(LPCVOID)(baseAddress + offSet), &lpBuf, sizeof(lpBuf),NULL);
		return lpBuf;
	}
	template<typename T>
	void WPM(uintptr_t baseAddress, T data)
	{
		WriteProcessMemory(globals::hproc,(LPVOID)baseAddress,&data,sizeof(data),NULL);
	}
	uintptr_t localplayerI;
	uintptr_t getLocalPlayer()
	{
		return RPM<uintptr_t>(hazedumper::signatures::dwLocalPlayer + globals::moduleBase);
	}

}