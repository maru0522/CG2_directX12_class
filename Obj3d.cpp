#include "Obj3d.h"
#include "InitDirectX.h"

Obj3d::Obj3d()
{
}

void Obj3d::Update(Matrix4& matView, Matrix4& matProjection)
{
    worldTransform.UpdateMatrix(matView, matProjection);
}

void Obj3d::Draw()
{
    InitDirectX* iDX_ = GetInstanceIDX();

    // ���_�o�b�t�@�̐ݒ�
    iDX_->GetCommandList()->IASetVertexBuffers(0, 1, mesh.GetVbView());
    // �C���f�b�N�X�o�b�t�@�̐ݒ�
    iDX_->GetCommandList()->IASetIndexBuffer(mesh.GetIbView());
    // �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
    iDX_->GetCommandList()->SetGraphicsRootConstantBufferView(2, worldTransform.GetCbTransMap()->GetBuffer()->GetGPUVirtualAddress());

    // �`��R�}���h���X�g
    iDX_->GetCommandList()->DrawIndexedInstanced(mesh.GetIndices()->size(), 1, 0, 0, 0);
}
