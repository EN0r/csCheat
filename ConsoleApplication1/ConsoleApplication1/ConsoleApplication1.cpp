
//cs cheat
#include <iostream>
#include "windows.h"
#include "hackMain.h"
#include "mFuncs.h"
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

int main()
{
    DWORD pid;
    globals::counterstrikeHandle = FindWindowA(NULL,"Counter-Strike: Global Offensive");
    globals::hdc = GetDC(FindWindowA(NULL,"Counter-Strike: Global Offensive"));
    GetWindowThreadProcessId(globals::counterstrikeHandle,&pid);

    globals::moduleBase = globals::getModule("client.dll",pid);
    globals::hproc = OpenProcess(PROCESS_ALL_ACCESS,NULL,pid);
    std::cout << globals::moduleBase << std::endl;
    
    Main::hackMain();


}