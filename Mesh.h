#pragma once
#include "Indispensable.h"
#include "ConstBuffer.h"

// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
struct ConstBufferDataMaterial {
    XMFLOAT4 color;		// �F�iRGBA�j
};

// ���_�f�[�^�\����
struct Vertex
{
    XMFLOAT3 pos;       // xyz���W
    XMFLOAT3 normal;    // �@���x�N�g��
    XMFLOAT2 uv;        // uv���W
};

class Mesh
{
public: // �����o�֐�
    Mesh();

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // �o�b�t�@�̍쐬
    ComPtr<ID3D12Resource> vertBuff = nullptr;
    ComPtr<ID3D12Resource> indexBuff = nullptr;

    // �r���[�̍쐬
    D3D12_VERTEX_BUFFER_VIEW vbView{};
    D3D12_INDEX_BUFFER_VIEW ibView{};

    // �萔�o�b�t�@
    ConstBuffer<ConstBufferDataMaterial> cbMaterialMap{};
};

