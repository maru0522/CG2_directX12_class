#pragma once
#include <Windows.h>        // CG2_01_01   12����
#include <cassert>          // CG2_01_02   ��
#include <d3d12.h>          // CG2_01_02   ��7����
#include <dxgi1_6.h>        // CG2_01_02   ��
#include <vector>           // CG2_01_02   ��12����
#include <string>           // CG2_01_02   ��
#include <DirectXMath.h>    // CG2_02_01   5����
#include <d3dcompiler.h>    // CG2_02_01   23����
#include <DirectXTex.h>     // CG2_04_03   11����


#pragma comment(lib, "d3d12.lib")           // Essentials.h   4�s��
#pragma comment(lib, "dxgi.lib")            // Essentials.h   5�s��
#pragma comment(lib, "d3dcompiler.lib")     // Essentials.h   9�s��


using namespace DirectX;


// �E�B���h�E�v���V�[�W��
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);