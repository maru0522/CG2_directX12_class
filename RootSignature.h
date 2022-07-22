#pragma once
#include "Indispensable.h"

class RootSignature
{
public: // �����o�֐�
    void Initialize();
    ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
    Comptr<ID3D12RootSignature> rootSignature;
};

RootSignature* GetInstanceRS();
