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
    public: // 静的なメンバ関数
        // シングルトンインスタンスの取得
        static Keyboard* GetInstance();

    public: // メンバ関数
        void Initialize();  
        void Update();

        bool isTrigger(UINT8 key);  // 押した瞬間
        bool isDown(UINT8 key);     // 押されているとき
        bool isReleased(UINT8 key); // 離された瞬間

    private: // メンバ変数
        // エイリアステンプレート
        template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

        Comptr<IDirectInput8> directInput = nullptr;
        Comptr<IDirectInputDevice8> keyboard = nullptr;
        std::array<BYTE, 256> preKeys;
        std::array<BYTE, 256> keys;

    private: // メンバ関数
        Keyboard() = default;
        ~Keyboard() = default;
        Keyboard(const Keyboard&) = delete;
        const Keyboard& operator=(const Keyboard&) = delete;
    };
};

