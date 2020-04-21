
// 预处理定义？
// #define _DLLAPI
// #include "testdll.h"
#include <windows.h>
#include <stdio.h>

// 隐式调用动态链接库
// #pragma comment(lib, "libhook.lib")
//声明命名空间std
// using namespace std;

// 有一个函数 他又两个参数
// 定义导入函数 自定义类型
typedef int (*PADD)(int a, int b);
typedef int (*PSUB)(int a, int b);

int main(int argc, char const *argv[])
{

    // 加载动态链接库
    HMODULE hDll = LoadLibrary(TEXT("b.dll"));
    // HMODULE hDlld = LoadLibrary(TEXT("libhook.so"));

    // 加载函数
    // 如果没有使用 extern "C" 导出函数 那么第二个参数可以为空
    // PADD pAdd = (PADD)GetProcAddress(hDll, NULL);
    PADD pAdd = (PADD)GetProcAddress(hDll, "add");
    // PADD pAdd2 = (PADD)GetProcAddress(hDll, "_Z3addii");
    // PADD pAdd3 = (PADD)GetProcAddress(hDll, "sub");

    if (pAdd == 0)
    {
        printf("加载失败");
        return 0;
    }

    // PADD pAddo = (PADD)GetProcAddress(hDll, "_Z3addii");

    // PSUB pSub = (PSUB)GetProcAddress(hDll, "sub");

    //定义 int 变量
    int x, y;
    scanf("%d", &x);
    scanf("%d", &y);
    // printf("a+b=%d\n", pAddo(x, y));
    printf("a+b=%d\n", pAdd(x, y));
    // printf("a+b=%d\n", add(x, y));

    // printf("a+b=%d\n", sub(x, y));

    //从控制台获取用户输入
    // cin >> x >> y;
    // 输出
    // cout << "%d + %d = " << add(x, y) << std::endl;
    // cin >> x >> y;
    // 释放动态库
    FreeLibrary(hDll);

    return 0;
}
