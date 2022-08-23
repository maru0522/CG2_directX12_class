#pragma once
#include "Indispensable.h"

// 頂点データ構造体
struct Vertex
{
    XMFLOAT3 pos;       // xyz座標
    XMFLOAT3 normal;    // 法線ベクトル
    XMFLOAT2 uv;        // uv座標
};

class Model
{
    Model();

private: // メンバ変数
// エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // バッファの作成
    ComPtr<ID3D12Resource> vertBuff = nullptr;
    ComPtr<ID3D12Resource> indexBuff = nullptr;

    // ビューの作成
    D3D12_VERTEX_BUFFER_VIEW vbView{};
    D3D12_INDEX_BUFFER_VIEW ibView{};
};

