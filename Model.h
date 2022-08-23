#pragma once
#include "Indispensable.h"

// ���_�f�[�^�\����
struct Vertex
{
    XMFLOAT3 pos;       // xyz���W
    XMFLOAT3 normal;    // �@���x�N�g��
    XMFLOAT2 uv;        // uv���W
};

class Model
{
    Model();

private: // �����o�ϐ�
// �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // �o�b�t�@�̍쐬
    ComPtr<ID3D12Resource> vertBuff = nullptr;
    ComPtr<ID3D12Resource> indexBuff = nullptr;

    // �r���[�̍쐬
    D3D12_VERTEX_BUFFER_VIEW vbView{};
    D3D12_INDEX_BUFFER_VIEW ibView{};
};

