#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace Input
{
    class Keyboard
    {
    private:
        IDirectInput8* directInput = nullptr;
        IDirectInputDevice8* keyboard = nullptr;


    public:
        //void Initialize();
    };
};

