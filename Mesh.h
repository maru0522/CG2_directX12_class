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

    // private�����o�ϐ�: vbView ���擾
    D3D12_VERTEX_BUFFER_VIEW* GetVbView() { return &vbView; }

    // private�����o�ϐ�: ibView ���擾
    D3D12_INDEX_BUFFER_VIEW* GetIbView() { return &ibView; }

    // private�����o�ϐ�: vertices ���擾
    std::vector<Vertex>* GetVertices() { return &vertices; }

    // private�����o�ϐ�: indices ���擾
    std::vector<uint16_t>* GetIndices() { return &indices; }

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

    // vertex
    std::vector<Vertex> vertices;
    // index
    std::vector<uint16_t> indices;
};

