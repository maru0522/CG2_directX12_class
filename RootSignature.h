#pragma once
#include "Indispensable.h"

class RootSignature
{
public:
public:
    void Initialize();

private:
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
    Comptr<ID3D12RootSignature> rootSignature;
private:
};

RootSignature* GetInstanceRS();
