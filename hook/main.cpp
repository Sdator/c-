#include <windows.h>

// ���ӵ� exe
#pragma comment(lib, "myhook.lib");

// ���� dll ����
extern "C" __MIDL_DECLSPEC_DLLIMPORT BOOL InstallMouseHook();

/*
���ӿ�����ο� ���ڱ������Ǵ�ui�� ��Ҫ���� gdi32.obj ��̬��
    https://blog.csdn.net/HopefulLight/article/details/52968217
    https://www.cnblogs.com/memset/p/gcc_l_order.html?tdsourcetag=s_pctim_aiomsg

*/

HHOOK g_mouseHook; //���Ӿ��

/*
 ������Ϣ������
 1 HWND hWnd ���ھ������
 2 UINT uMsg ������Ϣ       unsigned int �޷���int����
 3 WPARAM unsigned int
 4 LPARAM LONG
*/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* 
  ��깳�Ӵ�����
1 ��δ���
2 ��������
3 ������
*/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

// GUI �������
int WINAPI WinMain(HINSTANCE hIns, HINSTANCE hPre, LPSTR lpCmd, int nCmdShow)
{
    // 1 ��ƴ���
    // ��������
    TCHAR szAppName[] = TEXT("text");                       //����Ӧ�ַ��� ����Ĵ��뼴���Զ�ƥ�䵱ǰ���õı��뷽ʽ
    WNDCLASS wc = {0};                                      //��ʼ���ṹ��Ϊ�հ�
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //���ذ�ɫ��ˢ ���app����ɫ
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);               //���ؼ�ͷ���
    wc.hInstance = hIns;                                    //��ǰӦ�ó���ʵ����� ����Ϊ������
    wc.lpfnWndProc = WindowProc;                            //�ص�����
    wc.lpszClassName = szAppName;                           //����
    wc.style = CS_HREDRAW | CS_VREDRAW;                     //�������� ˮƽ�ʹ�ֱ�����仯�ػ�

    // 2 ע�ᴰ����
    RegisterClass(&wc);
    // 3 ��������
    // ��������
    // ��������
    // ���ڷ�� ���ڱ߿�| ������|���ڲ˵�|���|��С��
    // ����x��
    // ����y��
    // ���ڸ߶�
    // ���ڿ��
    // �����ھ��
    // ���ڲ˵����
    // ���������Ǹ����̾��
    // δ֪
    HWND hWnd = CreateWindow(szAppName, TEXT("����"), WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 500, 200, 800, 600, NULL, NULL, hIns, NULL);

    // 4.��ʾ�͸��´���
    ShowWindow(hWnd, SW_SHOW); //��ʾ����
    UpdateWindow(hWnd);        //���´���
    // 5 ��Ϣѭ��
    MSG msg;
    // ����Ϣ���л�ȡ��Ϣ��ŵ� msg
    // ��ȡ���и������Ӵ��ڵ���Ϣ
    // ��С��Ϣ�������Ϣ ���� ��д��ʾ��ȡ����
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // ���������ϢתΪ�ַ���Ϣ
        TranslateMessage(&msg);
        // ����Ϣ�ַ������ڴ�����
        DispatchMessage(&msg);
    }

    return 0;
}
// ������Ϣ������ �ص�
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps; //��ˢ
    HDC hDC;        //��ͼ���
    switch (uMsg)
    {
    case WM_PAINT: //���ڻ�����Ϣ
    {
        hDC = BeginPaint(hWnd, &ps);                           //��ʼ��ͼ
        HPEN hRedPan = CreatePen(PS_SOLID, 5, RGB(255, 0, 0)); //������ɫ����
        SelectObject(hDC, hRedPan);                            // ѡ�񻭱�
        Ellipse(hDC, 0, 0, 200, 200);                          //����һ��Բ
        EndPaint(hWnd, &ps);                                   //������ͼ
        break;
    }
    case WM_LBUTTONDOWN: //������������Ϣ
    {
        // ������Ϣ
        // box ������Ϣ
        // box ��ʾ
        // box �������� ȷ�� ȡ�� ����
        MessageBox(hWnd, TEXT("������"), TEXT("��ʾ"), MB_OK);
        break;
    }
    case WM_CREATE: //���ڴ�����Ϣ
    {
        /* 
        ��������
        1 ��������
        2 ������
        3 ʵ�����
        4 �߳�ID
        */
        g_mouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());
        break;
    }
    //����Ҽ�������Ϣ
    case WM_RBUTTONDOWN:
    {

        if (InstallMouseHook())
            SetWindowText(hWnd, TEXT("���ӳɹ�"));
        else
            SetWindowText(hWnd, TEXT("����ʧ��"));

        break;
    }
    //����ƶ���Ϣ
    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam); //ȡ��λ2�ֽ�
        int y = HIWORD(lParam); //ȡ��λ2�ֽ�
        WCHAR str[200];
        wsprintf((LPSTR)str, TEXT("����(%d,%d)"), x, y);
        SetWindowText(hWnd, (LPSTR)str); //���ô��ڱ���
        break;
    }
    case WM_CLOSE:           //���ڹر���Ϣ ���Ͻ�x
        DestroyWindow(hWnd); //���ٴ���
        break;
    case WM_DESTROY:        //����������Ϣ
        PostQuitMessage(0); // Ͷ��WM_QUIT��Ϣ ����Ϣѭ���˳�
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); //���ݸ�Ĭ�ϴ�����
}

/* 
  ��깳�Ӵ�����
1 ��δ���
2 ��������
3 ������
*/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    /*
    ���ݹ��Ӹ���һ��������
    1 ���Ӿ��
    2-3 ��Ӧ���Ӳ���
    */
    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}
// CUI ������� cmd �ն�
// int main(int argc, char const *argv[])
// {
//     HMODULE hMod = GetModuleHandleA(NULL);
//     PBYTE pAddr=(PBYTE)hMod;

//     printf("Hello World\n");
//     return 0;
// }
