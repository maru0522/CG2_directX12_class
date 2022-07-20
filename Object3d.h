#pragma once
#include "Indispensable.h"
#include "InitDirectX.h"

class Object3d
{
    // 定数バッファ用データ構造体（3D変換行列）
    struct ConstBufferDataTransform {
        XMMATRIX mat;		// 3D変換行列
    };

public: // メンバ変数

public: // メンバ関数
    void SetScale(const float x, const float y, const float z) { scale = { x, y, z }; }
    void SetRotate(const float x, const float y, const float z) { rotation = { x, y, z }; }
    void SetPos(const float x, const float y, const float z) { position = { x, y, z }; }

    void Initialize();
    void Update(const XMMATRIX& _matView, const XMMATRIX& _matProjection);
    void Draw(D3D12_VERTEX_BUFFER_VIEW& _vbView, D3D12_INDEX_BUFFER_VIEW& _ibView, UINT& _numIndices);

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    InitDirectX* iDX = InitDirectX::GetInstance();

    // 定数バッファ（行列用）
    Comptr<ID3D12Resource> constBuffTransform;
    // 定位数バッファマップ（行列用)
    ConstBufferDataTransform* constMapTransform;

    // アフィン変換情報
    XMFLOAT3 scale;
    XMFLOAT3 rotation;
    XMFLOAT3 position;
    // ワールド変換行列
    XMMATRIX matWorld;
    // 親オブジェクトへのポインタ
    Object3d* parent = nullptr;
private: // メンバ関数

};

