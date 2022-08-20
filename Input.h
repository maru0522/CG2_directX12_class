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
    public: // メンバ関数
        void Initialize();
        void Update();

        // 押した瞬間
        bool IsTrigger(UINT8 key) { return !preKeys[key] && keys[key]; }

        // 押されているとき
        bool IsDown(UINT8 key) { return keys[key]; }

        // 離された瞬間
        bool IsReleased(UINT8 key) { return preKeys[key] && !keys[key]; }

    private: // メンバ変数
        // エイリアステンプレート
        template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

        IDirectInput8* directInput = nullptr; // DirectInput生成
        IDirectInputDevice8* keyboard = nullptr; // キーボードデバイス生成
        std::array<BYTE, 256> preKeys{};
        std::array<BYTE, 256> keys{};
    };
    // インスタンスを取得
    Input::Keyboard* GetInstanceKeyboard();
};

