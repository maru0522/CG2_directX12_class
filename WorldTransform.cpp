#include "WorldTransform.h"
#include "InitDirectX.h"

WorldTransform::WorldTransform()
{
}

void WorldTransform::Update(Matrix4& matView, Matrix4& matProjection)
{
    Matrix4 matScale{}, matRot{}, matTrans{};

    // �X�P�[��
    matScale = Scale(scale);

    // ��]
    matRot = Identity();
    matRot *= RotateZ(rotation.z);
    matRot *= RotateX(rotation.x);
    matRot *= RotateY(rotation.y);

    // ���s�ړ��s��
    matTrans = Translate(position);

    // ���[���h�s��̍���
    matWorld = Identity();          // �ό`�����Z�b�g
    matWorld *= matScale;           // ���[���h�s��ɃX�P�[�����O�𔽉f
    matWorld *= matRot;             // ���[���h�s��ɉ�]�𔽉f
    matWorld *= matTrans;           // ���[���h�s��ɕ��s�ړ��𔽉f

    // �e�I�u�W�F�N�g�������
    if (parent != nullptr) {
        //�e�I�u�W�F�N�g�̃��[���h�s���������
        matWorld *= parent->matWorld;
    }

    // �萔�o�b�t�@�փf�[�^�]��
    cbTransMap.GetCBTypeMap()->matrix = matWorld * matView * matProjection;
}

void WorldTransform::Draw()
{
    InitDirectX* iDX_ = GetInstanceIDX();

    // ���_�o�b�t�@�̐ݒ�
    iDX_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
    // �C���f�b�N�X�o�b�t�@�̐ݒ�
    iDX_->GetCommandList()->IASetIndexBuffer(&ibView);
    // �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
    iDX_->GetCommandList()->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());

    // �`��R�}���h���X�g
    iDX_->GetCommandList()->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}
