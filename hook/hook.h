
// 预处理 防止重复加载库
// 如果没有定义库 就定义一个
#ifndef HOOK_H
#define HOOK_H
// // 预定义
// #ifndef _APIHOOK
// // 如果定了宏 _APIHOOK 就使用宏 APIHOOK 代替 __declspec(dllimport)
// #define APIHOOK __declspec(dllimport)
// #else
// #define APIHOOK __declspec(dllexport) //否则就使用宏 APIHOOK 代替 __declspec(dllimport)
// #endif

#include <windows.h>

// 钩子安装 
BOOL InstallMouseHook();
// extern "C" APIHOOK BOOL InstallMouseHook();

/* 
  鼠标钩子处理函数
1 如何处理
2 按键类型
3 按键码
*/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif