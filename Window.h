#pragma once
#include "Essentials.h"

class Window
{
public: // �ÓI�����o�ϐ�
	// �E�B���h�E�T�C�Y
	static const int width = 1280;
	static const int height = 720;

public: // �ÓI�����o�֐�
	static Window* GetInstance();


public: // �����o�ϐ�
	// ���b�Z�[�W
	MSG msg{};

public: // �����o�֐�
	// Window�N���X�̐ݒ�
	void CreateObj();
	void CreateObj(LPCWSTR title);
	void DeleteObj();

	// �E�B���h�E�Œ�p�̃��b�Z�[�W���[�v����
	// WM_QUIT�Ȃ�FALSE��Ԃ�
	bool IsKeep();


private: // �����o�ϐ�
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	// �E�B���h�E�n���h���̐���
	HWND hwnd;

private: // �����o�֐�
	// �V���O���g��
	Window();
	~Window();
	Window(const Window&) = delete;
	const Window& operator=(const Window&) = delete;
};
