#pragma once
#include <d3d12.h>      // CG2_01_02   7枚目
#include <dxgi1_6.h>    // CG2_01_02   7枚目
#include <vector>       // CG2_01_02   12枚目
#include <wrl.h>        // CG2_07_01   13枚目

class InitDirectX
{
public: // 静的関数
    static InitDirectX* GetInstance(); // インスタンスを取得

public: // 関数
    void Initialize();
    void PreDraw();
    void PostDraw();

    // privateメンバ変数: device を取得
    ID3D12Device* GetDevice() { return device.Get(); }

    // privateメンバ変数: commandList を取得
    ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }
private: // 関数
    void DebugLayer();      // DXGIDevice() で使用
    void DXGIDevice();      // アダプタの選別/デバイスの生成
    void Command();         // コマンドアロケータ/リスト/キューの生成
    void SwapChain();
    void RTVDescHeap();     // RTV用デスクリプタヒープの生成 + 生成するための設定（デスクリプタ）
    void RTV();             // RTVの生成 + 生成するための設定（デスクリプタ）
    void DepthBuffer();
    void Fence();

    void ClearRTV();        // RTVをクリア
    void ClearDepthBuff();  // 深度バッファをクリア

private: // 変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // DebugLayer() で初期化
    ComPtr<ID3D12Debug> debugController; // DXGIDevice() で使用

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
};