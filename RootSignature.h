#pragma once
#include "Indispensable.h"

class RootSignature
{
public:
public:
    void Initialize();
    ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

private:
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
    Comptr<ID3D12RootSignature> rootSignature;
private:
};

RootSignature* GetInstanceRS();
