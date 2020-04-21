#include <windows.h>

// 链接到 exe
#pragma comment(lib, "myhook.lib");

// 导入 dll 函数
extern "C" __MIDL_DECLSPEC_DLLIMPORT BOOL InstallMouseHook();

/*
链接库问题参考 由于本程序是带ui的 需要链接 gdi32.obj 静态库
    https://blog.csdn.net/HopefulLight/article/details/52968217
    https://www.cnblogs.com/memset/p/gcc_l_order.html?tdsourcetag=s_pctim_aiomsg

*/

HHOOK g_mouseHook; //钩子句柄

/*
 窗口消息处理函数
 1 HWND hWnd 窗口句柄类型
 2 UINT uMsg 窗口消息       unsigned int 无符号int类型
 3 WPARAM unsigned int
 4 LPARAM LONG
*/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* 
  鼠标钩子处理函数
1 如何处理
2 按键类型
3 按键码
*/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

// GUI 程序入口
int WINAPI WinMain(HINSTANCE hIns, HINSTANCE hPre, LPSTR lpCmd, int nCmdShow)
{
    // 1 设计窗口
    // 创建类名
    TCHAR szAppName[] = TEXT("text");                       //自适应字符集 无需改代码即可自动匹配当前设置的编码方式
    WNDCLASS wc = {0};                                      //初始化结构体为空白
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //加载白色画刷 填充app背景色
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);               //加载箭头光标
    wc.hInstance = hIns;                                    //当前应用程序实例句柄 设置为本程序
    wc.lpfnWndProc = WindowProc;                            //回调函数
    wc.lpszClassName = szAppName;                           //类名
    wc.style = CS_HREDRAW | CS_VREDRAW;                     //窗口类风格 水平和垂直发生变化重绘

    // 2 注册窗口类
    RegisterClass(&wc);
    // 3 创建窗口
    // 窗口类名
    // 窗口名字
    // 窗口风格 窗口边框| 标题栏|窗口菜单|最大化|最小化
    // 窗口x轴
    // 窗口y轴
    // 窗口高度
    // 窗口宽度
    // 父窗口句柄
    // 窗口菜单句柄
    // 窗口属于那个进程句柄
    // 未知
    HWND hWnd = CreateWindow(szAppName, TEXT("测试"), WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 500, 200, 800, 600, NULL, NULL, hIns, NULL);

    // 4.显示和更新窗口
    ShowWindow(hWnd, SW_SHOW); //显示窗口
    UpdateWindow(hWnd);        //更新窗口
    // 5 消息循环
    MSG msg;
    // 从消息队列获取消息存放到 msg
    // 获取所有父窗口子窗口的消息
    // 最小信息和最大消息 数量 填写表示获取所有
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // 将虚拟键消息转为字符消息
        TranslateMessage(&msg);
        // 将消息分发给窗口处理函数
        DispatchMessage(&msg);
    }

    return 0;
}
// 窗口消息处理函数 回调
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps; //笔刷
    HDC hDC;        //绘图句柄
    switch (uMsg)
    {
    case WM_PAINT: //窗口绘制消息
    {
        hDC = BeginPaint(hWnd, &ps);                           //开始绘图
        HPEN hRedPan = CreatePen(PS_SOLID, 5, RGB(255, 0, 0)); //创建红色画笔
        SelectObject(hDC, hRedPan);                            // 选择画笔
        Ellipse(hDC, 0, 0, 200, 200);                          //绘制一个圆
        EndPaint(hWnd, &ps);                                   //结束绘图
        break;
    }
    case WM_LBUTTONDOWN: //鼠标左键按下消息
    {
        // 发送消息
        // box 正文消息
        // box 提示
        // box 窗口类型 确认 取消 重试
        MessageBox(hWnd, TEXT("你点击了"), TEXT("提示"), MB_OK);
        break;
    }
    case WM_CREATE: //窗口创建消息
    {
        /* 
        创建钩子
        1 钩子类型
        2 处理函数
        3 实例句柄
        4 线程ID
        */
        g_mouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());
        break;
    }
    //鼠标右键按下消息
    case WM_RBUTTONDOWN:
    {

        if (InstallMouseHook())
            SetWindowText(hWnd, TEXT("钩子成功"));
        else
            SetWindowText(hWnd, TEXT("钩子失败"));

        break;
    }
    //鼠标移动消息
    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam); //取高位2字节
        int y = HIWORD(lParam); //取低位2字节
        WCHAR str[200];
        wsprintf((LPSTR)str, TEXT("坐标(%d,%d)"), x, y);
        SetWindowText(hWnd, (LPSTR)str); //设置窗口标题
        break;
    }
    case WM_CLOSE:           //窗口关闭消息 右上角x
        DestroyWindow(hWnd); //销毁窗口
        break;
    case WM_DESTROY:        //窗口销毁信息
        PostQuitMessage(0); // 投递WM_QUIT消息 让消息循环退出
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); //传递给默认处理函数
}

/* 
  鼠标钩子处理函数
1 如何处理
2 按键类型
3 按键码
*/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    /*
    传递钩子给下一个处理函数
    1 钩子句柄
    2-3 对应钩子参数
    */
    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}
// CUI 程序入口 cmd 终端
// int main(int argc, char const *argv[])
// {
//     HMODULE hMod = GetModuleHandleA(NULL);
//     PBYTE pAddr=(PBYTE)hMod;

//     printf("Hello World\n");
//     return 0;
// }
