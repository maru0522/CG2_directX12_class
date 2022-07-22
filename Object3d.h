#pragma once
#include "Indispensable.h"
#include "InitDirectX.h"
#include "ConstBuffer.h"
#include "Model.h"

class Object3d
{
    // 定数バッファ用データ構造体（3D変換行列）
    struct ConstBufferDataTransform {
        XMMATRIX mat;		// 3D変換行列
    };

    // 定数バッファ用データ構造体（マテリアル）
    struct ConstBufferDataMaterial {
        XMFLOAT4 color;		// 色（RGBA）
    };

public: // メンバ変数
    Model* model = nullptr;

public: // メンバ関数
    void SetScale(const float x, const float y, const float z) { scale = { x, y, z }; }
    void SetRotate(const float x, const float y, const float z) { rotation = { x, y, z }; }
    void SetPos(const float x, const float y, const float z) { position = { x, y, z }; }

    ConstBuffer<ConstBufferDataTransform> GetCbBuffMat() { return cbBuffMat; }
    ConstBuffer<ConstBufferDataMaterial> GetCbBuffMaterial() { return cbBuffMaterial; }

    void Initialize();
    void Update(const XMMATRIX& _matView, const XMMATRIX& _matProjection);
    void Draw();

    ~Object3d();

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    ConstBuffer<ConstBufferDataTransform> cbBuffMat; // CB.行列用
    ConstBuffer<ConstBufferDataMaterial> cbBuffMaterial; // CB.マテリアル用

    // アフィン変換情報
    XMFLOAT3 scale;
    XMFLOAT3 rotation;
    XMFLOAT3 position;
    // ワールド変換行列
    XMMATRIX matWorld;
    // 親オブジェクトへのポインタ
    Object3d* parent = nullptr;
};

