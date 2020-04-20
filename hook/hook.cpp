
#include "hook.h"

HINSTANCE g_hInstance;
HHOOK g_mouseHook;

// dll 动态链接库入口
int WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReverser)
{
    // 获取模块句柄存到全局变量
    g_hInstance = hInstance;
    return 0;
}

BOOL InstallMouseHook()
{
    // 钩子类型 处理函数 模块句柄 线程句柄
    // GetModuleHandle(TEXT("hook")) 获取模块句柄
    g_mouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hInstance, 0);
    // 返回 是否安装成功 
    if (g_mouseHook)
        return true;
    else
        return false;
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}