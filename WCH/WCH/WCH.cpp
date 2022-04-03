/* 
WCH.cpp : 定义应用程序的入口点。


Web-Class-Helper Main File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define _CRT_SECURE_NO_WARNINGS
#define WCH_VER "1.1.0"
#include<thread>
#include "framework.h"
#include "WCH.h"
#include "modules/apis.hpp"
#include "modules/commands.hpp"
#include "modules/file-process.hpp"
#include "modules/functions.hpp"
#include "modules/variables.hpp"
using namespace std;

extern const string Weekdayname[7];
extern multimap <int, pair <int, string>> mm;
extern int WCH_clock_num;
extern bool cmd_line;
extern bool anti_idle;
extern bool mode;
extern string op;
extern string UserName;
extern ifstream fin;
extern ofstream fout;

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WCH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WCH));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WCH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            
            AllocConsole();
            freopen("CONIN$", "r", stdin);
            freopen("CONOUT$", "w", stdout);
            
            thread ConsoleIO([&] 
                {
                    WCH_Init();
                    WCH_ReadData();
                    while (op != "quit") {
                        WCH_check_clock();
                        if (WCH_ShortCutKeyCheck() && !cmd_line) {
                            WCH_SetWindowStatus(true);
                            if (anti_idle == true) {
                                WCH_SetTrayStatus(true);
                                WCH_SetWindowPos(true);
                            }
                        }
                        if (cmd_line) {
                            WCH_CL_Init();
                            if (op == "add") {
                                WCH_add();
                            }
                            else if (op == "delete") {
                                WCH_delete();
                            }
                            else if (op == "change") {
                                WCH_change();
                            }
                            else if (op == "help") {
                                WCH_help();
                            }
                            else if (op == "ow") {
                                WCH_ow();
                            }
                            else if (op == "hide") {
                                WCH_SetWindowStatus(false);
                            }
                            else if (op == "game") {
                                WCH_game();
                            }
                            else if (op == "time") {
                                char tmp[21];
                                WCH_Time a = WCH_GetTime();
                                sprintf(tmp, "%04d/%02d/%02d %02d %02d %02d", a.Year, a.Month, a.Day, a.Hour, a.Minute, a.Second);
                                cout << tmp << endl << endl;
                            }
                            else if (op == "pi") {
                                WCH_pi();
                            }
                            else if (op == "mode") {
                                WCH_mode();
                            }
                            else if (op == "speedtest") {
                                WCH_speedtest();
                            }
                            else if (op == "anti-idle") {
                                WCH_anti_idle();
                            }
                            else if (op == "trans") {
                                WCH_trans();
                            }
                            else if (op != "quit") {
                                WCH_unknown();
                            }
                            cout << endl;
                        }
                    }
                    WCH_save();
                    FreeConsole();
                    return 0; 
                });
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
