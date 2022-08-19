#pragma once
#include "Indispensable.h"

class Window
{
public: // �ÓI�����o�ϐ�
    // �E�B���h�E�T�C�Y
    static const size_t width = 1280;
    static const size_t height = 720;

    // �E�B���h�E�N���X��
    static const wchar_t windowClassName[];

public: // �����o�֐�
    // �E�B���h�E�̐���
    Window();
    Window(const char* title);
    void DelWindow();

    // �E�B���h�E�Œ�p�̃��b�Z�[�W���[�v����
    // WM_QUIT�Ȃ�FALSE��Ԃ�
    bool IsKeep();

    // �E�B���h�E�n���h���̎擾
    // const: �����n�����郁���o�ϐ��̃����o�֐������ɂ�����ύX�̋֎~
    HWND GetHwnd() const { return hwnd; }

    HINSTANCE GetHInstance() const { return w.hInstance; }

private: // �����o�ϐ�
    // �E�B���h�E�N���X�̐���
    WNDCLASSEX w{};

    // �E�B���h�E�n���h���̐���
    HWND hwnd;
};

// �C���X�^���X���擾
Window* GetInstanceWnd();