#pragma once
#include <Windows.h>        // CG2_01_01   12枚目
#include <cassert>          // CG2_01_02   ┐
#include <d3d12.h>          // CG2_01_02   ├7枚目
#include <dxgi1_6.h>        // CG2_01_02   ┘
#include <vector>           // CG2_01_02   ┬12枚目
#include <string>           // CG2_01_02   ┘
#include <DirectXMath.h>    // CG2_02_01   5枚目
#include <d3dcompiler.h>    // CG2_02_01   23枚目
#include <DirectXTex.h>     // CG2_04_03   11枚目
#include <wrl.h>            // CG2_07_01   13枚目


#pragma comment(lib, "d3d12.lib")           // Indispensable.h   4行目
#pragma comment(lib, "dxgi.lib")            // Indispensable.h   5行目
#pragma comment(lib, "d3dcompiler.lib")     // Indispensable.h   9行目


using namespace DirectX;


LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// 静的メンバ変数
// 静的メンバ関数
// pubメンバ変数
// pubメンバ関数
// priメンバ変数
// priメンバ関数