#pragma once
#include "Indispensable.h"
#include "ConstBuffer.h"

// 定数バッファ用データ構造体（3D変換行列）
struct ConstBufferDataTransform {
    Matrix4 matrix;		// 3D変換行列
};

class WorldTransform
{
public: // メンバ変数
    // アフィン変換情報
    Vector3 scale = { 1,1,1 };
    Vector3 rotation = { 0,0,0 };
    Vector3 position = { 0,0,0 };

    // ワールド変換行列
    Matrix4 matWorld = {};

    // 親オブジェクトへのポインタ
    WorldTransform* parent = nullptr;

public: // メンバ関数
    WorldTransform();
    void UpdateMatrix(Matrix4& matView, Matrix4& matProjection);

    ConstBuffer<ConstBufferDataTransform>* GetCbTransMap() { return &cbTransMap; }

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // 定数バッファ（行列用）
    ComPtr<ID3D12Resource> cbTrans = nullptr;
    // 定数バッファマップ（行列用）
    ConstBuffer<ConstBufferDataTransform> cbTransMap{};

};

