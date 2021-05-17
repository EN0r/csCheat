#pragma once
#include "windows.h"
#include "csgo.hpp"
#include <Psapi.h>
#include <TlHelp32.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

namespace globalvars
{
	bool triggerbot = false;
	bool boneEsp = true;
	bool glowEsp = false;
	bool gdiEsp = false;
	bool aimAssit = true;
}


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
		vec3 operator- (const vec3& b) {
			this->x - b.x;
			this->y - b.y;
			this->z - b.z;
			return *this;
		}
		float x = 0;
		float y = 0;
		float z = 0;
	};

	struct vec2
	{
		float x = 0;
		float y = 0;
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

	void DrawPixel(float x, float y, int thickness)
	{
		HBRUSH brush = CreateSolidBrush(0x00FFFFFF);
		DrawFilledRect(x, y,thickness, thickness, brush);
	}

	void DrawText(std::string text,int x,int y, int w, int h, HBRUSH brush = CreateSolidBrush(0x00FFFFFF))
	{
		RECT rect = { x, y, w, h };
		DrawText(globals::hdc,text.c_str(),sizeof(text),&rect, DT_CALCRECT || DT_NOCLIP);
	}

	int Sign(float x) {
		if (x < 0)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}

	void DrawLine(float originx, float originy, float endx, float endy, float thickness, HBRUSH brush = CreateSolidBrush(0x00FFFFFF))
	{
		float distBetween = originx - endy;

		float deltax = endx - originx;
		float deltay = endy - originy;

		abs(deltax / deltay);
		
		int Y = originy;
		int eps = 0;

		

		for (int x = originx; x <= endx; x++)
		{
			DrawPixel(x,Y,thickness);
			eps += deltay;
			if ((eps << 1) >= deltax)
			{
				Y++;  eps -= deltax;
			}
		}

		//float x1 = originx;
		//float y1 = originy;
		//float x2 = endx;
		//float y2 = endy;
		//float dy = y2 - y1;
		//float dx = x2 - x1;

		//float y;
		//float x;

		//if (fabs(dy) > fabs(dx))
		//{
		//	for (y = y1; y != y2; y += Sign(dy))
		//	{
		//		x = x1 + (y - y1) * dx / dy;
		//		DrawPixel(x, y, thickness);

		//	}
		//}

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
	struct mat3x4
	{
		float* operator[ ](int index) {
			return c[index];
		}
		float c[3][4];
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

	void DrawBone(DWORD boneBase,mat4x4 viewMatrix, int bone1, int bone2)
	{
		globals::vec3 bone1_pos;
		globals::vec3 w2sb1;
		bone1_pos = GetBonePosition(boneBase, bone1);

		globals::vec3 bone2_pos;
		globals::vec3 w2sb2;
		bone1_pos = GetBonePosition(boneBase, bone2);

		w2sb1 = WorldToScreen(bone1_pos, viewMatrix);
		w2sb2 = WorldToScreen(bone2_pos, viewMatrix);

		DrawLine(w2sb1.x, w2sb1.y,w2sb2.x,w2sb2.y, 2);


	}


}