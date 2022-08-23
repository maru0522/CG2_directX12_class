#include "ConstBuffer.h"
#include "InitDirectX.h"

ConstBuffer::ConstBuffer()
{
    HRESULT result = S_FALSE;

    InitDirectX* iDX_ = GetInstanceIDX();

    // �萔�o�b�t�@�����p�̐ݒ�
    // �q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES cbHeapProp{};
    cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;				// GPU�ւ̓]���p
    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC cbResourceDesc{};
    cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
    cbResourceDesc.Height = 1;
    cbResourceDesc.DepthOrArraySize = 1;
    cbResourceDesc.MipLevels = 1;
    cbResourceDesc.SampleDesc.Count = 1;
    cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // �萔�o�b�t�@�̐���
    result = iDX_->GetDevice()->CreateCommittedResource(
        &cbHeapProp,	// �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &cbResourceDesc,	// ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&cBMaterial));
    assert(SUCCEEDED(result));

    // �萔�o�b�t�@�̃}�b�s���O
    CBMap();
    SetColor(1.0f, 1.0f, 1.0f, 1.0f); // color: white
}

ConstBuffer::~ConstBuffer()
{
    CBUnMap();
}

void ConstBuffer::CBMap()
{
    HRESULT result = S_FALSE;

    // �萔�o�b�t�@�̃}�b�s���O
    result = cBMaterial->Map(0, nullptr, (void**)&cMapMaterial);		// �}�b�s���O
    assert(SUCCEEDED(result));
}

void ConstBuffer::CBUnMap()
{
    cBMaterial->Unmap(0, nullptr);
}
