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
    public: // �ÓI�ȃ����o�֐�
        // �V���O���g���C���X�^���X�̎擾
        static Keyboard* GetInstance();

    public: // �����o�֐�
        void Initialize();  
        void Update();

        bool isTrigger(UINT8 key);  // �������u��
        bool isDown(UINT8 key);     // ������Ă���Ƃ�
        bool isReleased(UINT8 key); // �����ꂽ�u��

    private: // �����o�ϐ�
        // �G�C���A�X�e���v���[�g
        template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

        Comptr<IDirectInput8> directInput = nullptr;
        Comptr<IDirectInputDevice8> keyboard = nullptr;
        std::array<BYTE, 256> preKeys;
        std::array<BYTE, 256> keys;

    private: // �����o�֐�
        Keyboard() = default;
        ~Keyboard() = default;
        Keyboard(const Keyboard&) = delete;
        const Keyboard& operator=(const Keyboard&) = delete;
    };
};

