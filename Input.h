#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Indispensable.h"
#include "Window.h"
#include <array>
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace Input
{
    class Keyboard
    {
    public: // �����o�֐�
        void Initialize();
        void Update();

        // �������u��
        bool IsTrigger(UINT8 key) { return !preKeys[key] && keys[key]; }

        // ������Ă���Ƃ�
        bool IsDown(UINT8 key) { return keys[key]; }

        // �����ꂽ�u��
        bool IsReleased(UINT8 key) { return preKeys[key] && !keys[key]; }

    private: // �����o�ϐ�
        // �G�C���A�X�e���v���[�g
        template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

        IDirectInput8* directInput = nullptr; // DirectInput����
        IDirectInputDevice8* keyboard = nullptr; // �L�[�{�[�h�f�o�C�X����
        std::array<BYTE, 256> preKeys{};
        std::array<BYTE, 256> keys{};
    };
    // �C���X�^���X���擾
    Input::Keyboard* GetInstanceKeyboard();
};

