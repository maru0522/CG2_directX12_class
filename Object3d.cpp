#include "Object3d.h"

void Object3d::Initialize()
{
    HRESULT result;

    // �萔�o�b�t�@�̃q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES heapProp{};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    // �萔�o�b�t�@�̃��\�[�X�ݒ�
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // �萔�o�b�t�@�̐���
    result = InitDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffTransform)
    );
    assert(SUCCEEDED(result));

    // �萔�o�b�t�@�̃}�b�s���O
    result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
    assert(SUCCEEDED(result));
}

void Object3d::Update(const XMMATRIX& _matView, const XMMATRIX& _matProjection)
{
    XMMATRIX matScale, matRot, matTrans;

    // �X�P�[���A��]�A���s�ړ��s��̌v�Z
    matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
    matRot = XMMatrixIdentity();
    matRot *= XMMatrixRotationZ(rotation.z);
    matRot *= XMMatrixRotationX(rotation.x);
    matRot *= XMMatrixRotationY(rotation.y);
    matTrans = XMMatrixTranslation(
        position.x, position.y, position.x
    );

    // ���[���h�s��̍���
    matWorld = XMMatrixIdentity();  // �ό`�����Z�b�g
    matWorld *= matScale;           // ���[���h�s��ɃX�P�[�����O�𔽉f
    matWorld *= matRot;             // ���[���h�s��ɉ�]�𔽉f
    matWorld *= matTrans;           // ���[���h�s��ɕ��s�ړ��𔽉f

    // �e�I�u�W�F�N�g�������
    if (parent != nullptr) {
        //�e�I�u�W�F�N�g�̃��[���h�s���������
        matWorld *= parent->matWorld;
    }

    // �萔�o�b�t�@�փf�[�^�]��
    constMapTransform->mat = matWorld * _matView * _matProjection;
}

void Object3d::Draw(D3D12_VERTEX_BUFFER_VIEW& _vbView, D3D12_INDEX_BUFFER_VIEW& _ibView, UINT& _numIndices)
{
    // ���_�o�b�t�@�̐ݒ�
    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &_vbView);
    // �C���f�b�N�X�o�b�t�@�̐ݒ�
    iDX->GetCommandList()->IASetIndexBuffer(&_ibView);
    // �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

    // �`��R�}���h���X�g
    iDX->GetCommandList()->DrawIndexedInstanced(_numIndices, 1, 0, 0, 0);
}