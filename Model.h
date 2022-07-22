#pragma once
#include "Indispensable.h"
#include "ConstBuffer.h"

// 頂点データ構造体
struct Vertex
{
    XMFLOAT3 pos;       // xyz座標
    XMFLOAT3 normal;    // 法線ベクトル
    XMFLOAT2 uv;        // uv座標
};

class Model
{
public: // メンバ変数
    D3D12_VERTEX_BUFFER_VIEW vbView{}; // 頂点バッファビュー
    D3D12_INDEX_BUFFER_VIEW ibView{}; // インデックスバッファビュー

    UINT numIndecies;

public: // メンバ関数
    void Initialize();

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    Comptr<ID3D12Resource> vertBuff = nullptr; // 頂点バッファ
    Comptr<ID3D12Resource> indexBuff = nullptr; // インデックスバッファ
};

