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

    // �E�B���h�E�̃n���h��
    HWND hwnd;


public:
    // �E�B���h�E�I�u�W�F�N�g�̐���
    void CreateObj();
    void CreateObj(LPCWSTR title);
    void CreateObj(LPCWSTR title,int _width,int _height);

    // �E�B���h�E�N���X���iOS����H)�o�^����
    void DeleteObj();
};

