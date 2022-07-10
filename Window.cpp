#include "Window.h"

void Window::CreateObj()
{
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc;        // �E�B���h�E�v���V�[�W����ݒ�
    w.lpszClassName = L"DirectXGame";           // �E�B���h�E�N���X��
    w.hInstance = GetModuleHandle(nullptr);     // �E�B���h�E�n���h��
    w.hCursor = LoadCursor(NULL, IDC_ARROW);    // �J�[�\���w��


    // �E�B���h�E�N���X��OS�ɓo�^����
    RegisterClassEx(&w);


    // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
    RECT wrc = { 0, 0, width, height };
    // �����ŃT�C�Y��␳����
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


    // �E�B���h�E�I�u�W�F�N�g�̐���
    HWND hwnd = CreateWindow(w.lpszClassName,           // �N���X��
                             L"DirectXGame",            // �^�C�g���o�[�̕���
                             WS_OVERLAPPEDWINDOW,       // �W���I�ȃE�B���h�E�X�^�C��
                             CW_USEDEFAULT,             // �\��X���W�iOS�ɔC����j
                             CW_USEDEFAULT,             // �\��Y���W�iOS�ɔC����j
                             wrc.right - wrc.left,      // �E�B���h�E����
                             wrc.bottom - wrc.top,      // �E�B���h�E�c��
                             nullptr,                   // �e�E�B���h�E�n���h��
                             nullptr,                   // ���j���[�n���h��
                             w.hInstance,               // �Ăяo���A�v���P�[�V�����n���h��
                             nullptr);                  // �I�v�V����


    // �E�B���h�E��\����Ԃɂ���
    ShowWindow(hwnd, SW_SHOW);
}

void Window::CreateObj(LPCWSTR title)
{
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc;        // �E�B���h�E�v���V�[�W����ݒ�
    w.lpszClassName = title;                    // �E�B���h�E�N���X��
    w.hInstance = GetModuleHandle(nullptr);     // �E�B���h�E�n���h��
    w.hCursor = LoadCursor(NULL, IDC_ARROW);    // �J�[�\���w��


    // �E�B���h�E�N���X��OS�ɓo�^����
    RegisterClassEx(&w);


    // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
    RECT wrc = { 0, 0, width, height };
    // �����ŃT�C�Y��␳����
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


    // �E�B���h�E�I�u�W�F�N�g�̐���
    HWND hwnd = CreateWindow(w.lpszClassName,           // �N���X��
                             title,                     // �^�C�g���o�[�̕���
                             WS_OVERLAPPEDWINDOW,       // �W���I�ȃE�B���h�E�X�^�C��
                             CW_USEDEFAULT,             // �\��X���W�iOS�ɔC����j
                             CW_USEDEFAULT,             // �\��Y���W�iOS�ɔC����j
                             wrc.right - wrc.left,      // �E�B���h�E����
                             wrc.bottom - wrc.top,      // �E�B���h�E�c��
                             nullptr,                   // �e�E�B���h�E�n���h��
                             nullptr,                   // ���j���[�n���h��
                             w.hInstance,               // �Ăяo���A�v���P�[�V�����n���h��
                             nullptr);                  // �I�v�V����


    // �E�B���h�E��\����Ԃɂ���
    ShowWindow(hwnd, SW_SHOW);
}

void Window::CreateObj(LPCWSTR title, int _width, int _height)
{
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc;        // �E�B���h�E�v���V�[�W����ݒ�
    w.lpszClassName = title;                    // �E�B���h�E�N���X��
    w.hInstance = GetModuleHandle(nullptr);     // �E�B���h�E�n���h��
    w.hCursor = LoadCursor(NULL, IDC_ARROW);    // �J�[�\���w��


    // �E�B���h�E�N���X��OS�ɓo�^����
    RegisterClassEx(&w);


    // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
    RECT wrc = { 0, 0, _width, _height };
    // �����ŃT�C�Y��␳����
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


    // �E�B���h�E�I�u�W�F�N�g�̐���
    HWND hwnd = CreateWindow(w.lpszClassName,           // �N���X��
                             title,                     // �^�C�g���o�[�̕���
                             WS_OVERLAPPEDWINDOW,       // �W���I�ȃE�B���h�E�X�^�C��
                             CW_USEDEFAULT,             // �\��X���W�iOS�ɔC����j
                             CW_USEDEFAULT,             // �\��Y���W�iOS�ɔC����j
                             wrc.right - wrc.left,      // �E�B���h�E����
                             wrc.bottom - wrc.top,      // �E�B���h�E�c��
                             nullptr,                   // �e�E�B���h�E�n���h��
                             nullptr,                   // ���j���[�n���h��
                             w.hInstance,               // �Ăяo���A�v���P�[�V�����n���h��
                             nullptr);                  // �I�v�V����


    // �E�B���h�E��\����Ԃɂ���
    ShowWindow(hwnd, SW_SHOW);
}

void Window::DeleteObj()
{
    // �E�B���h�E�N���X��o�^����
    UnregisterClass(w.lpszClassName, w.hInstance);
}