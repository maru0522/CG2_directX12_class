#pragma once
#include "Indispensable.h"
#include "InitDirectX.h"
#include "ConstBuffer.h"
#include "Model.h"

class Object3d
{
    // �萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
    struct ConstBufferDataTransform {
        XMMATRIX mat;		// 3D�ϊ��s��
    };

    // �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
    struct ConstBufferDataMaterial {
        XMFLOAT4 color;		// �F�iRGBA�j
    };

public: // �����o�ϐ�
    Model* model = nullptr;

    // �A�t�B���ϊ����
    XMFLOAT3 scale = { 1,1,1 };
    XMFLOAT3 rotation = { 0,0,0 };
    XMFLOAT3 position = { 0,0,0 };
    // ���[���h�ϊ��s��
    XMMATRIX matWorld = XMMatrixIdentity();

public: // �����o�֐�
    void SetScale(const float x, const float y, const float z) { scale = { x, y, z }; }
    void SetRotate(const float x, const float y, const float z) { rotation = { x, y, z }; }
    void SetPos(const float x, const float y, const float z) { position = { x, y, z }; }

    ConstBuffer<ConstBufferDataTransform> GetCbBuffMat() { return cbBuffMat; }
    ConstBuffer<ConstBufferDataMaterial> GetCbBuffMaterial() { return cbBuffMaterial; }

    void Initialize();
    void Update(const XMMATRIX& _matView, const XMMATRIX& _matProjection);
    void Draw();

    ~Object3d();

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    ConstBuffer<ConstBufferDataTransform> cbBuffMat; // CB.�s��p
    ConstBuffer<ConstBufferDataMaterial> cbBuffMaterial; // CB.�}�e���A���p

    // �e�I�u�W�F�N�g�ւ̃|�C���^
    Object3d* parent = nullptr;
};

