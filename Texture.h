#pragma once
#include "Indispensable.h"

class Texture
{
public: // メンバ関数
    void LoadWICTex(std::string path);

    UINT GetIncrementSize() { return incrementSize; }

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    // 画像イメージデータ配列
    TexMetadata metadata{}; // 画像のサイズ等、テクスチャとしての各種情報が入る
    ScratchImage scratchImg{}; // ビットマップ形式での画像データそのもののアドレスが入る

    Comptr<ID3D12Resource> texBuff = nullptr;

    UINT incrementSize;
};

