#pragma once
#include "Indispensable.h"

template <typename CBType> class ConstBuffer
{
public: // メンバ関数
    ConstBuffer();
    ~ConstBuffer();

    // マテリアルカラーのセッティング
    CBType* GetCBTypeMap() { return &cbTypeMap }

    // privateメンバ変数: cBMaterial を取得
    ID3D12Resource* GetBuffer() { return buffer.Get(); }

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12Resource> buffer = nullptr;

    CBType* cbTypeMap = nullptr;

private: // メンバ関数
    // cBMaterialとcMapMaterialのマッピング
    void CBMap();

    // 途中で色を変化させる予定がないなら初期化後に使う
    // マッピングの解除
    void CBUnMap();
};

