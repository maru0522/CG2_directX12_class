#pragma once
#include "Indispensable.h"
#include "InitDirectX.h"

class Object3d
{
    // �萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
    struct ConstBufferDataTransform {
        XMMATRIX mat;		// 3D�ϊ��s��
    };

public: // �����o�ϐ�

public: // �����o�֐�
    void SetScale(const float x, const float y, const float z) { scale = { x, y, z }; }
    void SetRotate(const float x, const float y, const float z) { rotation = { x, y, z }; }
    void SetPos(const float x, const float y, const float z) { position = { x, y, z }; }

    void Initialize();
    void Update(const XMMATRIX& _matView, const XMMATRIX& _matProjection);
    void Draw(D3D12_VERTEX_BUFFER_VIEW& _vbView, D3D12_INDEX_BUFFER_VIEW& _ibView, UINT& _numIndices);

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    InitDirectX* iDX = InitDirectX::GetInstance();

    // �萔�o�b�t�@�i�s��p�j
    Comptr<ID3D12Resource> constBuffTransform;
    // ��ʐ��o�b�t�@�}�b�v�i�s��p)
    ConstBufferDataTransform* constMapTransform;

    // �A�t�B���ϊ����
    XMFLOAT3 scale;
    XMFLOAT3 rotation;
    XMFLOAT3 position;
    // ���[���h�ϊ��s��
    XMMATRIX matWorld;
    // �e�I�u�W�F�N�g�ւ̃|�C���^
    Object3d* parent = nullptr;
private: // �����o�֐�

};

