#include "ConstBuffer.h"
#include "InitDirectX.h"

template<typename CBType>
ConstBuffer<CBType>::ConstBuffer()
{
    HRESULT result = S_FALSE;

    InitDirectX* iDX_ = GetInstanceIDX();

    // 定数バッファ生成用の設定
    // ヒープ設定
    D3D12_HEAP_PROPERTIES cbHeapProp{};
    cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;				// GPUへの転送用
    // リソース設定
    D3D12_RESOURCE_DESC cbResourceDesc{};
    cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    cbResourceDesc.Width = (sizeof(CBType) + 0xff) & ~0xff;	//256バイトアラインメント
    cbResourceDesc.Height = 1;
    cbResourceDesc.DepthOrArraySize = 1;
    cbResourceDesc.MipLevels = 1;
    cbResourceDesc.SampleDesc.Count = 1;
    cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 定数バッファの生成
    result = iDX_->GetDevice()->CreateCommittedResource(
        &cbHeapProp,	// ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &cbResourceDesc,	// リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer));
    assert(SUCCEEDED(result));

    // 定数バッファのマッピング
    CBMap();
    //SetColor(1.0f, 1.0f, 1.0f, 1.0f); // color: white
}

template<typename CBType>
ConstBuffer<CBType>::~ConstBuffer()
{
    CBUnMap();
}

template<typename CBType>
void ConstBuffer<CBType>::CBMap()
{
    HRESULT result = S_FALSE;

    // 定数バッファのマッピング
    result = buffer->Map(0, nullptr, (void**)&cbTypeMap);		// マッピング
    assert(SUCCEEDED(result));
}

template<typename CBType>
void ConstBuffer<CBType>::CBUnMap()
{
    buffer->Unmap(0, nullptr);
}
