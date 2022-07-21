#pragma once
#include "Indispensable.h"
#include "ConstBuffer.h"

// 定数バッファ用データ構造体（マテリアル）
struct ConstBufferDataMaterial {
    XMFLOAT4 color;		// 色（RGBA）
};

// 頂点データ構造体
struct Vertex
{
    XMFLOAT3 pos;       // xyz座標
    XMFLOAT3 normal;    // 法線ベクトル
    XMFLOAT2 uv;        // uv座標
};

class Model
{
public:
public:
    void Initialize();

private:
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    D3D12_VERTEX_BUFFER_VIEW vbView{}; // 頂点バッファビュー
    D3D12_INDEX_BUFFER_VIEW ibView{}; // インデックスバッファビュー

    Comptr<ID3D12Resource> vertBuff = nullptr; // 頂点バッファ
    Comptr<ID3D12Resource> indexBuff = nullptr; // インデックスバッファ
private:
};

