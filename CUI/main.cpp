#include <windows.h>
#include <cstdio>

// CUI ³ÌÐòÈë¿Ú cmd ÖÕ¶Ë
int main(int argc, char const *argv[])
{
    HMODULE hMod = GetModuleHandleA(NULL);
    PBYTE pAddr = (PBYTE)hMod;

    printf("Hello World\n");
    return 0;
}
