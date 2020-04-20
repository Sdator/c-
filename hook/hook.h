
#include <windows.h>

// 定义并导出 dll 函数
extern "C" __MIDL_DECLSPEC_DLLEXPORT BOOL InstallMouseHook();

/* 
  鼠标钩子处理函数
1 如何处理
2 按键类型
3 按键码
*/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);


