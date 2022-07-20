#pragma once
#include "Indispensable.h"

template <typename CBType> class ConstBuffer
{
    // 定数バッファ用データ構造体（3D変換行列）
    struct ConstBufferDataTransform {
        XMMATRIX mat;		// 3D変換行列
    };
public: // メンバ変数
public: // メンバ関数
    void Create();

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    // 定数バッファ（行列用）
    Comptr<ID3D12Resource> ConstBuff = nullptr;

private: // メンバ関数
};

