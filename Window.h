#pragma once
#include "Indispensable.h"

class Window
{
public: // 静的メンバ変数
    // ウィンドウサイズ
    static const size_t width = 1280;
    static const size_t height = 720;

    // ウィンドウクラス名
    static const wchar_t windowClassName[];

public: // メンバ関数
    // ウィンドウの生成
    Window();
    Window(const char* title);
    void DelWindow();

    // ウィンドウ固定用のメッセージループ処理
    // WM_QUITならFALSEを返す
    bool IsKeep();

    // ウィンドウハンドルの取得
    // const: 引き渡しするメンバ変数のメンバ関数内部における変更の禁止
    HWND GetHwnd() const { return hwnd; }

    HINSTANCE GetHInstance() const { return w.hInstance; }

private: // メンバ変数
    // ウィンドウクラスの生成
    WNDCLASSEX w{};

    // ウィンドウハンドルの生成
    HWND hwnd;
};

// インスタンスを取得
Window* GetInstanceWnd();