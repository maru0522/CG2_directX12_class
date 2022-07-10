#include "Window.h"

void Window::CreateObj()
{
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc;        // ウィンドウプロシージャを設定
    w.lpszClassName = L"DirectXGame";           // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr);     // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW);    // カーソル指定


    // ウィンドウクラスをOSに登録する
    RegisterClassEx(&w);


    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, width, height };
    // 自動でサイズを補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


    // ウィンドウオブジェクトの生成
    HWND hwnd = CreateWindow(w.lpszClassName,           // クラス名
                             L"DirectXGame",            // タイトルバーの文字
                             WS_OVERLAPPEDWINDOW,       // 標準的なウィンドウスタイル
                             CW_USEDEFAULT,             // 表示X座標（OSに任せる）
                             CW_USEDEFAULT,             // 表示Y座標（OSに任せる）
                             wrc.right - wrc.left,      // ウィンドウ横幅
                             wrc.bottom - wrc.top,      // ウィンドウ縦幅
                             nullptr,                   // 親ウィンドウハンドル
                             nullptr,                   // メニューハンドル
                             w.hInstance,               // 呼び出しアプリケーションハンドル
                             nullptr);                  // オプション


    // ウィンドウを表示状態にする
    ShowWindow(hwnd, SW_SHOW);
}

void Window::CreateObj(LPCWSTR title)
{
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc;        // ウィンドウプロシージャを設定
    w.lpszClassName = title;                    // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr);     // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW);    // カーソル指定


    // ウィンドウクラスをOSに登録する
    RegisterClassEx(&w);


    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, width, height };
    // 自動でサイズを補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


    // ウィンドウオブジェクトの生成
    HWND hwnd = CreateWindow(w.lpszClassName,           // クラス名
                             title,                     // タイトルバーの文字
                             WS_OVERLAPPEDWINDOW,       // 標準的なウィンドウスタイル
                             CW_USEDEFAULT,             // 表示X座標（OSに任せる）
                             CW_USEDEFAULT,             // 表示Y座標（OSに任せる）
                             wrc.right - wrc.left,      // ウィンドウ横幅
                             wrc.bottom - wrc.top,      // ウィンドウ縦幅
                             nullptr,                   // 親ウィンドウハンドル
                             nullptr,                   // メニューハンドル
                             w.hInstance,               // 呼び出しアプリケーションハンドル
                             nullptr);                  // オプション


    // ウィンドウを表示状態にする
    ShowWindow(hwnd, SW_SHOW);
}

void Window::CreateObj(LPCWSTR title, int _width, int _height)
{
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc;        // ウィンドウプロシージャを設定
    w.lpszClassName = title;                    // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr);     // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW);    // カーソル指定


    // ウィンドウクラスをOSに登録する
    RegisterClassEx(&w);


    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, _width, _height };
    // 自動でサイズを補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


    // ウィンドウオブジェクトの生成
    HWND hwnd = CreateWindow(w.lpszClassName,           // クラス名
                             title,                     // タイトルバーの文字
                             WS_OVERLAPPEDWINDOW,       // 標準的なウィンドウスタイル
                             CW_USEDEFAULT,             // 表示X座標（OSに任せる）
                             CW_USEDEFAULT,             // 表示Y座標（OSに任せる）
                             wrc.right - wrc.left,      // ウィンドウ横幅
                             wrc.bottom - wrc.top,      // ウィンドウ縦幅
                             nullptr,                   // 親ウィンドウハンドル
                             nullptr,                   // メニューハンドル
                             w.hInstance,               // 呼び出しアプリケーションハンドル
                             nullptr);                  // オプション


    // ウィンドウを表示状態にする
    ShowWindow(hwnd, SW_SHOW);
}

void Window::DeleteObj()
{
    // ウィンドウクラスを登録解除
    UnregisterClass(w.lpszClassName, w.hInstance);
}