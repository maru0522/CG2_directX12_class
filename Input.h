#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Indispensable.h"
#include "Window.h"
#include <dinput.h>
#include <array>
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
        bool isTrigger(UINT8 key) { return !preKeys[key] && keys[key]; }

        // ������Ă���Ƃ�
        bool isDown(UINT8 key) { return keys[key]; }

        // �����ꂽ�u��
        bool isReleased(UINT8 key) { return preKeys[key] && !keys[key]; }

    private: // �����o�ϐ�
        // �G�C���A�X�e���v���[�g
        template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

        Comptr<IDirectInput8> directInput = nullptr;
        Comptr<IDirectInputDevice8> keyboard = nullptr;
        std::array<BYTE, 256> preKeys;
        std::array<BYTE, 256> keys;
    };

    Keyboard* GetInstanceKeys();
};

