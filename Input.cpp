#include "Input.h"

Input::Keyboard* Input::Keyboard::GetInstance()
{
    static Keyboard instance;
    return &instance;
}

void Input::Keyboard::Initialize()
{
    Window* window = Window::GetInstance();

    // DirectInput�̏�����
    HRESULT result = DirectInput8Create(
        window->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
        (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    // �L�[�{�[�h�f�o�C�X�̐���
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    assert(SUCCEEDED(result));

    // ���̓f�[�^�`���̃Z�b�g
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
    assert(SUCCEEDED(result));

    // �r�����䃌�x���̃Z�b�g
    result = keyboard->SetCooperativeLevel(
        window->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));
}

void Input::Keyboard::Update()
{
    // prekyes�֏��ۑ�
    for (int i = 0; i < 256; i++) {
        preKeys[i] = keys[i];
    }


    // �L�[�{�[�h���̎擾�J�n
    keyboard->Acquire();

    // �R���e�i�̒��g��S��0�Ŗ��߂�
    keys.fill(0);
    // �S�L�[�̓��͏�Ԃ��擾����
    keyboard->GetDeviceState(static_cast<DWORD>(size(keys)),keys.data());
}

bool Input::Keyboard::isTrigger(UINT8 key)
{
    return !preKeys[key] && keys[key];
}

bool Input::Keyboard::isDown(UINT8 key)
{
    return keys[key];
}

bool Input::Keyboard::isReleased(UINT8 key)
{
    return preKeys[key] && !keys[key];
}