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

class Mesh
{
public: // メンバ関数
    Mesh();

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // バッファの作成
    ComPtr<ID3D12Resource> vertBuff = nullptr;
    ComPtr<ID3D12Resource> indexBuff = nullptr;

    // ビューの作成
    D3D12_VERTEX_BUFFER_VIEW vbView{};
    D3D12_INDEX_BUFFER_VIEW ibView{};

    // 定数バッファ
    ConstBuffer<ConstBufferDataMaterial> cbMaterialMap{};
};

