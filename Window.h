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

	// ���b�Z�[�W
	MSG msg{};

public:
	// Window�N���X�̐ݒ�
	void CreateObj();
	void CreateObj(LPCWSTR title);			// �g���ۂ́iL"title"�j�ƋL�q���邱��
	void DeleteObj();

	// �E�B���h�E�Œ�p�̃��b�Z�[�W���[�v����
	// WM_QUIT�Ȃ�FALSE��Ԃ�
	bool IsKeep();
};
