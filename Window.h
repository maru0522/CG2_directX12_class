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

    // ウィンドウのハンドル
    HWND hwnd;


public:
    // ウィンドウオブジェクトの生成
    void CreateObj();
    void CreateObj(LPCWSTR title);
    void CreateObj(LPCWSTR title,int _width,int _height);

    // ウィンドウクラスを（OSから？)登録解除
    void DeleteObj();
};

