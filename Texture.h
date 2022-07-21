#pragma once
#include "Indispensable.h"

class Texture
{
public:
public:
    void LoadWICTex(std::string path);

private:
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    // 画像イメージデータ配列
    TexMetadata metadata{}; // 画像のサイズ等、テクスチャとしての各種情報が入る
    ScratchImage scratchImg{}; // ビットマップ形式での画像データそのもののアドレスが入る

    Comptr<ID3D12Resource> texBuff = nullptr;
    Comptr<ID3D12DescriptorHeap> srvHeap = nullptr;

private:
};

