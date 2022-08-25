#include "InitDirectX.h"

void InitDirectX::Initialize()
{
    DXGIDevice();
    Command(); // コマンドアロケータ/リスト/キューの生成
    SwapChain();
    SrvDescHeap(); // SRV用のデスクリプタヒープの生成 + 生成するための設定（デスクリプタ）
    RtvDescHeap(); // RTV用のデスクリプタヒープの生成 + 生成するための設定（デスクリプタ）
    //Srv(); // SRVの生成 + 生成するための設定（デスクリプタ）
    Rtv(); // RTVの生成 + 生成するための設定（デスクリプタ）
    DepthBuffer();
    Fence();
}

void InitDirectX::PreDraw()
{
    // ウィンドウのインスタンス取得
    Window* win_ = GetInstanceWnd();

#pragma region リソースバリアの変更
    // バックバッファの番号を取得（2つなので0番か1番）
    UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

    // １．リソースバリアで書き込み可能に変更
    D3D12_RESOURCE_BARRIER barrierDesc{};
    barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // バックバッファを指定
    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
    commandList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 描画先の変更
    // ２．描画先の変更
    // レンダーターゲットビューのハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvDescHeapDesc.Type);
    // 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
    // レンダーターゲットをセット
    commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion
    
#pragma region 画面クリア
    // ３．画面クリア 
    ClearRTV();
    // 深度バッファクリア
    ClearDepthBuff();
#pragma endregion

#pragma region グラフィックスコマンド入力前設定
    // ４．描画コマンドここから
    // ビューポート設定コマンド
    D3D12_VIEWPORT viewport{};
    viewport.Width = win_->width;
    viewport.Height = win_->height;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    // ビューポート設定コマンドを、コマンドリストに積む
    commandList->RSSetViewports(1, &viewport);

    // シザー矩形
    D3D12_RECT scissorRect{};
    scissorRect.left = 0; // 切り抜き座標左
    scissorRect.right = scissorRect.left + win_->width; // 切り抜き座標右
    scissorRect.top = 0; // 切り抜き座標上
    scissorRect.bottom = scissorRect.top + win_->height; // 切り抜き座標下
    // シザー矩形設定コマンドを、コマンドリストに積む
    commandList->RSSetScissorRects(1, &scissorRect);
#pragma endregion
    // ----- ↓↓↓PreDraw()とPostDraw()の間に描画コマンドを書き込む↓↓↓ ----- //
}

void InitDirectX::PostDraw()
{
    // ----- ↑↑↑PreDraw()とPostDraw()の間に描画コマンドを書き込む↑↑↑ ----- //
    // ４．描画コマンドここまで

    HRESULT result = S_FALSE;

    // バックバッファの番号を取得（2つなので0番か1番）
    UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

#pragma region リソースバリアを元に戻す
// ５．リソースバリアを戻す
    D3D12_RESOURCE_BARRIER barrierDesc{};
    barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // バックバッファを指定
    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
    commandList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region コマンドリスト実行
    // 命令のクローズ
    result = commandList->Close();
    assert(SUCCEEDED(result));

    // コマンドリストの実行
    ID3D12CommandList* commandLists[] = { commandList.Get() };
    commandQueue->ExecuteCommandLists(1, commandLists);
    // 画面に表示するバッファをフリップ（裏表の入替え）
    result = swapChain->Present(1, 0);
    assert(SUCCEEDED(result));


    // コマンドの実行完了を待つ
    commandQueue->Signal(fence.Get(), ++fenceVal);
    if (fence->GetCompletedValue() != fenceVal) {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        if (event != 0) {
            fence->SetEventOnCompletion(fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
    }
    // キューをクリア
    result = cmdAllocator->Reset();
    assert(SUCCEEDED(result));
    // 再びコマンドリストを貯める準備
    result = commandList->Reset(cmdAllocator.Get(), nullptr);
    assert(SUCCEEDED(result));
#pragma endregion
}

void InitDirectX::DXGIDevice()
{
    HRESULT result = S_FALSE;

#pragma region デバッグレイヤ
#ifdef _DEBUG
    //デバッグレイヤーをオンに
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
    }
#endif
#pragma endregion

#pragma region アダプタの列挙
    // DXGIファクトリーの生成
    result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
    assert(SUCCEEDED(result));

    // アダプターの列挙用
    std::vector<ComPtr<IDXGIAdapter4>> adapters;

    // ここに特定の名前を持つアダプターオブジェクトが入る
    ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

    // パフォーマンスが高いものから順に、全てのアダプターを列挙する
    for (UINT i = 0;
         dxgiFactory->
         EnumAdapterByGpuPreference(i,
         DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
         IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
         i++) {
        // 動的配列に追加する
        adapters.push_back(tmpAdapter);
    }
#pragma endregion

#pragma region アダプタの選別
    // 妥当なアダプタを選別する
    for (size_t i = 0; i < adapters.size(); i++) {
        DXGI_ADAPTER_DESC3 adapterDesc;

        // アダプターの情報を取得する
        adapters[i]->GetDesc3(&adapterDesc);

        // ソフトウェアデバイスを回避
        if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
            // デバイスを採用してループを抜ける
            tmpAdapter = adapters[i];
            break;
        }
    }
#pragma endregion 

#pragma region デバイスの生成
    // 対応レベルの配列
    D3D_FEATURE_LEVEL levels[] = {
     D3D_FEATURE_LEVEL_12_1,
     D3D_FEATURE_LEVEL_12_0,
     D3D_FEATURE_LEVEL_11_1,
     D3D_FEATURE_LEVEL_11_0,
    };


    D3D_FEATURE_LEVEL featureLevel;
    for (size_t i = 0; i < _countof(levels); i++) {
        // 採用したアダプターでデバイスを生成
        result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
                                   IID_PPV_ARGS(&device));
        if (result == S_OK) {
            // デバイスを生成できた時点でループを抜ける
            featureLevel = levels[i];
            break;
        }
    }
#pragma endregion
}

void InitDirectX::Command()
{
    HRESULT result = S_FALSE;

#pragma region コマンドアロケータの生成
    // コマンドアロケータを生成
    result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&cmdAllocator));
    assert(SUCCEEDED(result));
#pragma endregion

#pragma region コマンドリストの生成
    // コマンドリストを生成
    result = device->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr,IID_PPV_ARGS(&commandList));
    assert(SUCCEEDED(result));
#pragma endregion

#pragma region コマンドキューの生成
    //コマンドキューの設定
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    //コマンドキューを生成
    result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
    assert(SUCCEEDED(result));
#pragma endregion
}

void InitDirectX::SwapChain()
{
    HRESULT result = S_FALSE;

    // ウィンドウのインスタンス取得
    Window* win_ = GetInstanceWnd();

#pragma region スワップチェーンの生成
    // スワップチェーンの設定
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = 1280;
    swapChainDesc.Height = 720;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
    swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
    swapChainDesc.BufferCount = 2; // バッファ数を２つに設定
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // IDXGISwapChain1のComptrを用意
    ComPtr<IDXGISwapChain1> swapChain1;

    // スワップチェーンの生成
    result = dxgiFactory->CreateSwapChainForHwnd(
        commandQueue.Get(), win_->GetHwnd(), &swapChainDesc, nullptr, nullptr,
        &swapChain1);
    assert(SUCCEEDED(result));

    // 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
    swapChain1.As(&swapChain);
#pragma endregion
}

void InitDirectX::RtvDescHeap()
{
    HRESULT result = S_FALSE;

    DXGI_SWAP_CHAIN_DESC swcDesc = {};
    result = swapChain->GetDesc(&swcDesc);
    assert(SUCCEEDED(result));

    // デスクリプタヒープの設定
    rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
    rtvDescHeapDesc.NumDescriptors = swcDesc.BufferCount; // 裏表の２つ

    // 設定を元にRTV用デスクリプタヒープの生成
    device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&rtvHeap));
}

void InitDirectX::Rtv()
{
    HRESULT result = S_FALSE;

    DXGI_SWAP_CHAIN_DESC swcDesc = {};
    result = swapChain->GetDesc(&swcDesc);
    assert(SUCCEEDED(result));

    backBuffers.resize(swcDesc.BufferCount);

    // スワップチェーンの全てのバッファについて処理する
    for (size_t i = 0; i < backBuffers.size(); i++) {
        // スワップチェーンからバッファを取得
        swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
        // デスクリプタヒープのハンドルを取得
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
        // 裏か表かでアドレスがずれる
        rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvDescHeapDesc.Type);
        // レンダーターゲットビューの設定
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
        // シェーダーの計算結果をSRGBに変換して書き込む
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        // レンダーターゲットビューの生成
        device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
    }
}

void InitDirectX::DepthBuffer()
{
    HRESULT result = S_FALSE;

    // ウィンドウのインスタンス取得
    Window* win_ = GetInstanceWnd();

#pragma region 深度テスト深度バッファ
    // リソース設定
    D3D12_RESOURCE_DESC depthResourceDesc{};
    depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthResourceDesc.Width = win_->width;     // レンダーターゲットに合わせる
    depthResourceDesc.Height = win_->height;   // レンダーターゲットに合わせる
    depthResourceDesc.DepthOrArraySize = 1;
    depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;   // 深度値フォーマット
    depthResourceDesc.SampleDesc.Count = 1;
    depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;  // デプスステンシル

    // 深度値用ヒーププロパティ
    D3D12_HEAP_PROPERTIES depthHeapProp{};
    depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

    // 深度値のクリア設定
    D3D12_CLEAR_VALUE depthClearValue{};
    depthClearValue.DepthStencil.Depth = 1.0f;      // 深度値1.0f（最大値）でクリア
    depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
    // 深度バッファの生成
    // リソース生成
    result = device->CreateCommittedResource(
        &depthHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &depthResourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,       // 深度値書き込みに使用
        &depthClearValue,
        IID_PPV_ARGS(&depthBuff)
    );

    // 深度ビュー用デスクリプタヒープの設定
    D3D12_DESCRIPTOR_HEAP_DESC dsvDescHeapDesc{};
    dsvDescHeapDesc.NumDescriptors = 1;     // 深度ビューは1つ
    dsvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;      // デプスステンシルビュー
    // 設定を元に深度ビュー用デスクリプタヒープの生成
    result = device->CreateDescriptorHeap(&dsvDescHeapDesc, IID_PPV_ARGS(&dsvHeap));

    // 深度ビューの設定
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;     // 深度値フォーマット
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    // 深度ビューの生成
    device->CreateDepthStencilView(
        depthBuff.Get(),
        &dsvDesc,
        dsvHeap->GetCPUDescriptorHandleForHeapStart());
#pragma endregion
}

void InitDirectX::Fence()
{
    HRESULT result = S_FALSE;

    result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void InitDirectX::ClearRTV()
{
    // バックバッファの番号を取得（2つなので0番か1番）
    UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

    // レンダーターゲットビューのハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvDescHeapDesc.Type);

    // 何色でクリアするのか設定
    FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
    // RTVをクリアする関数をコマンドリストに積む
    commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void InitDirectX::ClearDepthBuff()
{
    // 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

    // 深度バッファをクリアする関数をコマンドリストに積む
    commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

static InitDirectX iDX;
InitDirectX* GetInstanceIDX()
{
    return &iDX;
}
