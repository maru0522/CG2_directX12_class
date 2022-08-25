#pragma once
#include "Indispensable.h"
#include "Window.h"

class InitDirectX
{
public: // メンバ関数
    void Initialize();
    void PreDraw();
    void PostDraw();

    // privateメンバ変数: device を取得
    ID3D12Device* GetDevice() { return device.Get(); }

    // privateメンバ変数: commandList を取得
    ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // DXGIDevice() で初期化
    ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
    ComPtr<ID3D12Device> device = nullptr;

    // Coomand() で初期化
    ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
    ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
    ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
 
    // SwapChain() で初期化
    ComPtr<IDXGISwapChain4> swapChain = nullptr;
    
    // RtvDescHeap() で初期化
    ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};

    // Rtv() で初期化
    std::vector<ComPtr<ID3D12Resource>> backBuffers; // バックバッファ

    // DepthBuffer() で初期化
    ComPtr<ID3D12Resource> depthBuff = nullptr;
    ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

    // Fence() で初期化
    ComPtr<ID3D12Fence> fence = nullptr;
    UINT64 fenceVal = 0;

private: // メンバ関数
    void DXGIDevice(); // アダプタの選別/デバイスの生成
    void Command(); // コマンドアロケータ/リスト/キューの生成
    void SwapChain();
    void RtvDescHeap(); // RTV用のデスクリプタヒープの生成 + 生成するための設定（デスクリプタ）
    // void Srv(); // SRVの生成 + 生成するための設定（デスクリプタ）
    void Rtv(); // RTVの生成 + 生成するための設定（デスクリプタ）
    void DepthBuffer();
    void Fence();

    void ClearRTV(); // RTVをクリア
    void ClearDepthBuff(); // 深度バッファをクリア
};
// インスタンスを取得
InitDirectX* GetInstanceIDX();

