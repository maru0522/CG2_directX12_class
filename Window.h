#pragma once
#include "Essentials.h"

class Window
{
public:
	// ウィンドウサイズ
	const int width = 1280;
	const int height = 720;

	// ウィンドウクラスの設定
	WNDCLASSEX w{};
	// ウィンドウハンドルの生成
	HWND hwnd;

public:
	// Windowクラスの設定
	void CreateObj();
	void CreateObj(LPCWSTR title);			// 使う際は（L"title"）と記述すること
	void DeleteObj();
};

