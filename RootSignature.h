#pragma once
#include "Indispensable.h"

class RootSignature
{
public: // メンバ関数
    void Initialize();
    ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
    Comptr<ID3D12RootSignature> rootSignature;
};

RootSignature* GetInstanceRS();
