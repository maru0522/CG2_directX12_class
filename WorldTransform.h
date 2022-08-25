#pragma once
#include "Model.h"
#include "Indispensable.h"

// �萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
struct ConstBufferDataTransform {
    Matrix4 matrix;		// 3D�ϊ��s��
};

class WorldTransform
{
public:
    WorldTransform();
    void Update(Matrix4& matView, Matrix4& matProjection);
    void Draw();

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // �萔�o�b�t�@�i�s��p�j
    ComPtr<ID3D12Resource> cbTrans = nullptr;
    // �萔�o�b�t�@�}�b�v�i�s��p�j
    ConstBuffer<ConstBufferDataTransform> cbTransMap{};

    // �A�t�B���ϊ����
    Vector3 scale = { 1,1,1 };
    Vector3 rotation = { 0,0,0 };
    Vector3 position = { 0,0,0 };

    // ���[���h�ϊ��s��
    Matrix4 matWorld = {};

    // �e�I�u�W�F�N�g�ւ̃|�C���^
    WorldTransform* parent = nullptr;
};

