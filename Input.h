#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Essentials.h"
#include "Window.h"
#include <dinput.h>
#include <array>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace Input
{
    class Keyboard
    {
    public: // �ÓI�ȃ����o�֐�
        static Keyboard* GetInstance();

    public: // �����o�֐�
        void Initialize();
        void Update();

        bool isTrigger(UINT8 key);
        bool isDown(UINT8 key);
        bool isReleased(UINT8 key);

    private: // �����o�ϐ�
        IDirectInput8* directInput = nullptr;
        IDirectInputDevice8* keyboard = nullptr;
        std::array<BYTE, 256> preKeys;
        std::array<BYTE, 256> keys;

    private: // �����o�֐�
        Keyboard() = default;
        ~Keyboard() = default;
        Keyboard(const Keyboard&) = delete;
        const Keyboard& operator=(const Keyboard&) = delete;
    };
};

