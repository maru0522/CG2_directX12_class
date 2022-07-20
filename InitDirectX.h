#pragma once
#include "Indispensable.h"

class InitDirectX
{
public: // メンバ関数
    void Initialize(); // DirectX初期化処理

    void PreDraw(); // 描画前処理

    void PostDraw(); // 描画後処理

    void ClearRTV(); // RTVのクリア

    void ClearDepthBuffer(); // 深度バッファのクリア

    ID3D12Device* GetDevice() { return device.Get(); } // privateメンバ変数: device を取得

    ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); } // privateメンバ変数: commandList を取得

    ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap.Get(); } // privateメンバ変数: srvHeap を取得

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    Comptr<IDXGIFactory7> dxgiFactory = nullptr;
    Comptr<ID3D12Device> device = nullptr;
    Comptr<ID3D12CommandAllocator> cmdAllocator = nullptr;
    Comptr<ID3D12GraphicsCommandList> commandList = nullptr;
    Comptr<ID3D12CommandQueue> commandQueue = nullptr;
    Comptr<IDXGISwapChain4> swapChain = nullptr;
    Comptr<ID3D12DescriptorHeap> srvHeap = nullptr;
    Comptr<ID3D12DescriptorHeap> rtvHeap = nullptr;
    std::vector<Comptr<ID3D12Resource>> backBuffers;
    Comptr<ID3D12Resource> depthBuff = nullptr;
    Comptr<ID3D12DescriptorHeap> dsvHeap = nullptr;
    Comptr<ID3D12Fence> fence = nullptr;
    UINT64 fenceVal = 0;

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
    D3D12_RESOURCE_BARRIER barrierDesc{};

private: // メンバ関数

    // デバイスの生成
    void DXGIDevice();
    // コマンドアロケータ/リスト/キューの生成
    void Command();
    // スワップチェーンの生成
    void SwapChain();
    // SRVDescHeapの生成
    void SRVDescHeap();
    // RTVDescHeapの生成
    void RTVDescHeap();
    // 深度バッファの生成
    void DepthBuffer();
    // フェンスの生成
    void Fence();
};

// インスタンスを取得
InitDirectX* GetInstanceIDX();
