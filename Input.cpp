#include "Input.h"

void Input::Keyboard::Initialize()
{
    // DirectInputの初期化
    HRESULT result = DirectInput8Create(
        GetInstanceWnd()->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    // キーボードデバイスの生成
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    assert(SUCCEEDED(result));

    // 入力データ形式のセット
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(result));

    // 排他制御レベルのセット
    result = keyboard->SetCooperativeLevel(
        GetInstanceWnd()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));
}

void Input::Keyboard::Update()
{
    // prekyesへ情報保存
    for (int i = 0; i < 256; i++) {
        preKeys[i] = keys[i];
    }


    // キーボード情報の取得開始
    keyboard->Acquire();

    // コンテナの中身を全て0で埋める
    keys.fill(0);
    // 全キーの入力状態を取得する
    keyboard->GetDeviceState(static_cast<DWORD>(size(keys)),keys.data());
}

static Input::Keyboard _keyboard;
Input::Keyboard* Input::GetInstanceKeys()
{
    return &_keyboard;
}