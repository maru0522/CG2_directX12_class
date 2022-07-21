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

class Model
{
public:
public:
    void Initialize();

private:
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    D3D12_VERTEX_BUFFER_VIEW vbView{}; // ���_�o�b�t�@�r���[
    D3D12_INDEX_BUFFER_VIEW ibView{}; // �C���f�b�N�X�o�b�t�@�r���[

    Comptr<ID3D12Resource> vertBuff = nullptr; // ���_�o�b�t�@
    Comptr<ID3D12Resource> indexBuff = nullptr; // �C���f�b�N�X�o�b�t�@
private:
};

