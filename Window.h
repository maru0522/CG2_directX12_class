#pragma once
#include "Essentials.h"

class Window
{
public:
	// �E�B���h�E�T�C�Y
	const int width = 1280;
	const int height = 720;

	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	// �E�B���h�E�n���h���̐���
	HWND hwnd;

public:
	// Window�N���X�̐ݒ�
	void CreateObj();
};

