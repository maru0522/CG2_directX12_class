#pragma once
#include "Indispensable.h"

// ���_���C�A�E�g
static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
    { // xyz���W(1�s�ŏ������ق������₷��)
        "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    },
    { // �@���x�N�g���i1�s�������ق����킩��₷��)
        "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
    },
    { // uv���W(1�s�ŏ������ق������₷��)
        "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    },
};

class PipelineStateObject
{
public:
public:
    void LoadVertShader(std::string _vertShader);
    void LoadPixelShader(std::string _pixelShader);
    void Initialize(std::string _vertShader, std::string _pixelShader);
private:
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
    Comptr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
    Comptr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g

    // �p�C�v�����X�e�[�g�̐���
    Comptr<ID3D12PipelineState> pipelineState = nullptr;
private:
};

