#pragma once
#include "Essentials.h"

class Window
{
public: // 静的メンバ変数
	// ウィンドウサイズ
	static const int width = 1280;
	static const int height = 720;

	// ウィンドウクラス名
	static const wchar_t windowClassName[];

public: // 静的メンバ関数
	// シングルトンインスタンスの取得
	static Window* GetInstance();

public: // メンバ関数
	// Windowクラスの設定
	void CreateObj();
	void CreateObj(const char* title);
	void DeleteObj();

	// ウィンドウ固定用のメッセージループ処理
	// WM_QUITならFALSEを返す
	bool IsKeep();

	// ウィンドウハンドルの取得
	// const: 引き渡しするメンバ変数のメンバ関数内部における変更の禁止
	HWND GetHwnd() const { return hwnd; }

	HINSTANCE GetHInstance() const { return w.hInstance; }


private: // メンバ変数
	// ウィンドウクラスの設定
	WNDCLASSEX w{};
	// ウィンドウハンドルの生成
	HWND hwnd;

private: // メンバ関数
	// シングルトン
	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	const Window& operator=(const Window&) = delete;
};
