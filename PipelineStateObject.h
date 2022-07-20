#pragma once
#include "Indispensable.h"

// 頂点レイアウト
static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
    { // xyz座標(1行で書いたほうが見やすい)
        "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
    },
    { // 法線ベクトル（1行書いたほうがわかりやすい)
        "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,
        D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
    },
    { // uv座標(1行で書いたほうが見やすい)
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
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
    Comptr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
    Comptr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト

    // パイプランステートの生成
    Comptr<ID3D12PipelineState> pipelineState = nullptr;
private:
};

