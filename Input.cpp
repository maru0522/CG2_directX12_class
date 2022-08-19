#include "Input.h"

void Input::Keyboard::Initialize()
{
    HRESULT result = S_FALSE;

    // DirectInput�̏�����
    result = DirectInput8Create(
        win_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    // �L�[�{�[�h�f�o�C�X�̐���
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    assert(SUCCEEDED(result));

    // ���̓f�[�^�`���̃Z�b�g
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
    assert(SUCCEEDED(result));

    // �r�����䃌�x���̃Z�b�g
    result = keyboard->SetCooperativeLevel(
        win_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
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
    keyboard->GetDeviceState(static_cast<DWORD>(size(keys)), keys.data());
}

static Input::Keyboard keyboard;
Input::Keyboard* Input::GetInstanceKeyboard()
{
    return &keyboard;
}
