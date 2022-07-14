#pragma once
#include "Essentials.h"

class Window
{
public: // 静的メンバ変数
	// ウィンドウサイズ
	static const int width = 1280;
	static const int height = 720;

public: // 静的メンバ関数
	static Window* GetInstance();


public: // メンバ変数
	// メッセージ
	MSG msg{};

public: // メンバ関数
	// Windowクラスの設定
	void CreateObj();
	void CreateObj(LPCWSTR title);
	void DeleteObj();

	// ウィンドウ固定用のメッセージループ処理
	// WM_QUITならFALSEを返す
	bool IsKeep();


private: // メンバ変数
	// ウィンドウクラスの設定
	WNDCLASSEX w{};
	// ウィンドウハンドルの生成
	HWND hwnd;

private: // メンバ関数
	// シングルトン
	Window();
	~Window();
	Window(const Window&) = delete;
	const Window& operator=(const Window&) = delete;
};
