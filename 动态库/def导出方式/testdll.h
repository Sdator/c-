// 预处理 防止重复加载库
// 如果没有定义库 就定义一个
#ifndef TESTDLL_H
#define TESTDLL_H
// 条件编译
// 方便引入头文件的时候 自动设置为导入还是导出函数
// 初始编写dll的时候引入文件就是导出函数
// 如果是其他程序引用这个头文件就设置为导入函数
#ifndef _DLLAPI
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

// 导出函数
// int DLLAPI add(int a, int b);            // 对应读取方式  PADD pAdd2 = (PADD)GetProcAddress(hDll, "_Z3addii"); 名字会混淆
extern "C" DLLAPI int add(int a, int b);
extern "C" DLLAPI int sub(int a, int b); // 对应读取方式  PADD pAdd3 = (PADD)GetProcAddress(hDll, "sub"); c语言模式名字正常

#endif
