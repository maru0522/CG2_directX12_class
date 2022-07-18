#pragma once
#include "Essentials.h"

class Window
{
public: // �ÓI�����o�ϐ�
	// �E�B���h�E�T�C�Y
	static const int width = 1280;
	static const int height = 720;

	// �E�B���h�E�N���X��
	static const wchar_t windowClassName[];

public: // �ÓI�����o�֐�
	// �V���O���g���C���X�^���X�̎擾
	static Window* GetInstance();

public: // �����o�֐�
	// Window�N���X�̐ݒ�
	void CreateObj();
	void CreateObj(const char* title);
	void DeleteObj();

	// �E�B���h�E�Œ�p�̃��b�Z�[�W���[�v����
	// WM_QUIT�Ȃ�FALSE��Ԃ�
	bool IsKeep();

	// �E�B���h�E�n���h���̎擾
	// const: �����n�����郁���o�ϐ��̃����o�֐������ɂ�����ύX�̋֎~
	HWND GetHwnd() const { return hwnd; }

	HINSTANCE GetHInstance() const { return w.hInstance; }


private: // �����o�ϐ�
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	// �E�B���h�E�n���h���̐���
	HWND hwnd;

private: // �����o�֐�
	// �V���O���g��
	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	const Window& operator=(const Window&) = delete;
};
