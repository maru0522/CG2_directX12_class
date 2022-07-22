#pragma once
#include "Indispensable.h"
#include "InitDirectX.h"

template <typename CBType> class ConstBuffer
{
public: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    // �萔�o�b�t�@
    Comptr<ID3D12Resource> constBuff = nullptr;

public: // �����o�֐�
    void Create() {
        HRESULT result;

        // �萔�o�b�t�@�̃q�[�v�ݒ�
        D3D12_HEAP_PROPERTIES heapProp{};
        heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
        // �萔�o�b�t�@�̃��\�[�X�ݒ�
        D3D12_RESOURCE_DESC resDesc{};
        resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resDesc.Width = (sizeof(CBType) + 0xff) & ~0xff;
        resDesc.Height = 1;
        resDesc.DepthOrArraySize = 1;
        resDesc.MipLevels = 1;
        resDesc.SampleDesc.Count = 1;
        resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // �萔�o�b�t�@�̐���
        result = GetInstanceIDX()->GetDevice()->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constBuff)
        );
        assert(SUCCEEDED(result));

        // �萔�o�b�t�@�̃}�b�s���O
        result = constBuff->Map(0, nullptr, (void**)&cbTypeMap);
        assert(SUCCEEDED(result));
    }
    void Delete();

    CBType* GetBuffMap() { return cbTypeMap; }

private: // �����o�ϐ�
    // �萔�o�b�t�@�̃}�b�v
    CBType* cbTypeMap = nullptr;
};

