#pragma once
#include <Windows.h>        // CG2_01_01   12–‡–Ú
#include <cassert>          // CG2_01_02   „¢
#include <d3d12.h>          // CG2_01_02   „¥7–‡–Ú
#include <dxgi1_6.h>        // CG2_01_02   „£
#include <vector>           // CG2_01_02   „¦12–‡–Ú
#include <string>           // CG2_01_02   „£
#include <DirectXMath.h>    // CG2_02_01   5–‡–Ú
#include <d3dcompiler.h>    // CG2_02_01   23–‡–Ú
#include <DirectXTex.h>     // CG2_04_03   11–‡–Ú


#pragma comment(lib, "d3d12.lib")           // Essentials.h   4s–Ú
#pragma comment(lib, "dxgi.lib")            // Essentials.h   5s–Ú
#pragma comment(lib, "d3dcompiler.lib")     // Essentials.h   9s–Ú


using namespace DirectX;


LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);