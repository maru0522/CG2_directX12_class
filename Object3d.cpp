#include "Object3d.h"

void Object3d::Initialize()
{
    model = new Model;

    cbBuffMat.Create();
    cbBuffMaterial.Create();
    model->Initialize();
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
    cbBuffMat.GetBuffMap()->mat = matWorld * _matView * _matProjection;
}

void Object3d::Draw()
{
    // ���_�o�b�t�@�̐ݒ�
    GetInstanceIDX()->GetCommandList()->IASetVertexBuffers(0, 1, &model->vbView);
    // �C���f�b�N�X�o�b�t�@�̐ݒ�
    GetInstanceIDX()->GetCommandList()->IASetIndexBuffer(&model->ibView);
    // �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
    GetInstanceIDX()->GetCommandList()->SetGraphicsRootConstantBufferView(2, GetCbBuffMat().constBuff->GetGPUVirtualAddress());

    // �`��R�}���h���X�g
    GetInstanceIDX()->GetCommandList()->DrawIndexedInstanced(model->numIndecies, 1, 0, 0, 0);
}

Object3d::~Object3d()
{
    delete model;
}
