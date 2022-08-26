#pragma once
#include "Indispensable.h"

struct Texture
{
    // 名前
    std::string name;
    // テクスチャ情報
    Microsoft::WRL::ComPtr<ID3D12Resource> buff = nullptr;
    // ハンドル
    D3D12_CPU_DESCRIPTOR_HANDLE srvCPUHandle{}; // CPU
    D3D12_GPU_DESCRIPTOR_HANDLE srvGPUHandle{}; // GPU
};

class TextureManager
{
public: // 静的メンバ関数
    //static void LoadTexture(const std::string& fileName);

private: // 静的メンバ変数
    // SRVの最大個数
    static const size_t maxSRVDesc = 256;

public: // メンバ関数
    TextureManager();

    void Load(const std::string& fileName);

    // 既に読み込んだテクスチャのSRVのハンドルを返す
    const Texture* GetTextureHandle(const std::string& fileName);

    // privateメンバ変数: srvHeap を取得
    ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap.Get(); }

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

    const std::string directoryPath = "Resource/";

    // SRVコンテナ
    std::array<Texture, maxSRVDesc> textures{};

    uint32_t indexNextDescHeap = 0;

};
// インスタンスを取得
TextureManager* GetInstanceTexM();

