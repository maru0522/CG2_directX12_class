#include "Window.h"

namespace {
    // ワイド文字に変換
    std::wstring ConvertString(const std::string& text) {
        if (text.empty()) {
            return std::wstring();
        }

        /*
         * ref: https://docs.microsoft.com/ja-JP/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
         * ref: http://www.t-net.ne.jp/~cyfis/win_api/sdk/MultiByteToWideChar.html
         * ref: http://chokuto.ifdef.jp/urawaza/api/MultiByteToWideChar.html
         */
        int wcharNum =
            MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), NULL, 0);
        if (wcharNum == 0) {
            return std::wstring();
        }
        std::wstring result(wcharNum, 0);
        MultiByteToWideChar(
            CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), &result[0], wcharNum);
        return result;
    }
}

Window* Window::GetInstance()
{
    static Window instance;
    return &instance;
}

const wchar_t Window::windowClassName[] = L"mEngine";

void Window::CreateObj()
{
    // ウィンドウクラスの設定
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc;        // ウィンドウプロシージャを設定
    w.lpszClassName = windowClassName;           // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr);     // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW);    // カーソル指定

    // ウィンドウクラスをOSに登録する
    RegisterClassEx(&w);

    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, width, height };
    // 自動でサイズを補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウオブジェクトの生成
    hwnd = CreateWindow(w.lpszClassName,        //クラス名
                        windowClassName,         // タイトルバーの文字
                        WS_OVERLAPPEDWINDOW,    // 標準的なウィンドウスタイル
                        CW_USEDEFAULT,          // 表示X座標 (OSに任せる)
                        CW_USEDEFAULT,          // 表示Y座標 (OSに任せる)
                        wrc.right - wrc.left,   // ウィンドウ横幅
                        wrc.bottom - wrc.top,   // ウィンドウ縦幅
                        nullptr,                // 親ウィンドウハンドル
                        nullptr,                // メニューハンドル
                        w.hInstance,            // 呼び出しアプリケーションハンドル
                        nullptr);               // オプション

    // ウィンドウを表示状態にする
    ShowWindow(hwnd, SW_SHOW);
}

void Window::CreateObj(const char* title) {
    // ウィンドウクラスの設定
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc;        // ウィンドウプロシージャを設定
    w.lpszClassName = windowClassName;     // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr);     // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW);    // カーソル指定

    // ウィンドウクラスをOSに登録する
    RegisterClassEx(&w);

    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, width, height };
    // 自動でサイズを補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウタイトルをwchar_tに変換
    std::wstring titleWStr = ConvertString(title);

    // ウィンドウオブジェクトの生成
    hwnd = CreateWindow(w.lpszClassName,        //クラス名
                        titleWStr.c_str(),                  // タイトルバーの文字
                        WS_OVERLAPPEDWINDOW,    // 標準的なウィンドウスタイル
                        CW_USEDEFAULT,          // 表示X座標 (OSに任せる)
                        CW_USEDEFAULT,          // 表示Y座標 (OSに任せる)
                        wrc.right - wrc.left,   // ウィンドウ横幅
                        wrc.bottom - wrc.top,   // ウィンドウ縦幅
                        nullptr,                // 親ウィンドウハンドル
                        nullptr,                // メニューハンドル
                        w.hInstance,            // 呼び出しアプリケーションハンドル
                        nullptr);               // オプション

    // ウィンドウを表示状態にする
    ShowWindow(hwnd, SW_SHOW);
}

void Window::DeleteObj()
{
    UnregisterClass(w.lpszClassName, w.hInstance);
}

bool Window::IsKeep()
{
    // メッセージ
    MSG msg{};

    // メッセージがある？
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
    {
        TranslateMessage(&msg);     // キー入力メッセージの処理
        DispatchMessage(&msg);      // プロシージャにメッセージを送る
    }

    // ✖ボタンで終了メッセージが来たらゲームループを抜ける
    return msg.message != WM_QUIT;
}

