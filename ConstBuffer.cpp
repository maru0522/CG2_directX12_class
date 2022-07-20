#include "ConstBuffer.h"

template<typename CBType> void ConstBuffer<CBType>::Create()
{
    HRESULT result;

    // 定数バッファのヒープ設定
    D3D12_HEAP_PROPERTIES heapProp{};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    // 定数バッファのリソース設定
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = (sizeof(CBType) + 0xff) & ~0xff;
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 定数バッファの生成
    result = InitDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuff)
    );
    assert(SUCCEEDED(result));

    // 定数バッファのマッピング
    result = constBuff->Map(0, nullptr, (void**)&CBType);
    assert(SUCCEEDED(result));
}

template<typename CBType> void ConstBuffer<CBType>::Delete()
{
    ConstBuff->Unmap(0, nullptr);
}