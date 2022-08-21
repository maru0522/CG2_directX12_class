#pragma once
#include "Indispensable.h"

// 定数バッファ用データ構造体（マテリアル）
struct ConstBufferDataMaterial {
    XMFLOAT4 color;		// 色（RGBA）
};

class ConstBuffer
{
public: // メンバ関数
    ConstBuffer();

    // マテリアルカラーのセッティング
    void SetColor(float R, float G, float B, float A) { cMapMaterial->color = XMFLOAT4(R, G, B, A); }

    // privateメンバ変数: cBMaterial を取得
    ID3D12Resource* GetCBMaterial() { return cBMaterial.Get(); }

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12Resource> cBMaterial = nullptr;
    ConstBufferDataMaterial* cMapMaterial = nullptr;

private: // メンバ関数
    // cBMaterialとcMapMaterialのマッピング
    void CBMap();

    // 途中で色を変化させる予定がないなら初期化後に使う
    // マッピングの解除
    void CBUnMap();
};

