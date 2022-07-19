#include "Object3d.h"

void Object3d::Initialize()
{
    HRESULT result;

    // 定数バッファのヒープ設定
    D3D12_HEAP_PROPERTIES heapProp{};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    // 定数バッファのリソース設定
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
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
        IID_PPV_ARGS(&constBuffTransform)
    );
    assert(SUCCEEDED(result));

    // 定数バッファのマッピング
    result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
    assert(SUCCEEDED(result));
}

void Object3d::Update(const XMMATRIX& _matView, const XMMATRIX& _matProjection)
{
    XMMATRIX matScale, matRot, matTrans;

    // スケール、回転、平行移動行列の計算
    matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
    matRot = XMMatrixIdentity();
    matRot *= XMMatrixRotationZ(rotation.z);
    matRot *= XMMatrixRotationX(rotation.x);
    matRot *= XMMatrixRotationY(rotation.y);
    matTrans = XMMatrixTranslation(
        position.x, position.y, position.x
    );

    // ワールド行列の合成
    matWorld = XMMatrixIdentity();  // 変形をリセット
    matWorld *= matScale;           // ワールド行列にスケーリングを反映
    matWorld *= matRot;             // ワールド行列に回転を反映
    matWorld *= matTrans;           // ワールド行列に平行移動を反映

    // 親オブジェクトがあれば
    if (parent != nullptr) {
        //親オブジェクトのワールド行列をかける
        matWorld *= parent->matWorld;
    }

    // 定数バッファへデータ転送
    constMapTransform->mat = matWorld * _matView * _matProjection;
}

void Object3d::Draw(D3D12_VERTEX_BUFFER_VIEW& _vbView, D3D12_INDEX_BUFFER_VIEW& _ibView, UINT& _numIndices)
{
    // 頂点バッファの設定
    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &_vbView);
    // インデックスバッファの設定
    iDX->GetCommandList()->IASetIndexBuffer(&_ibView);
    // 定数バッファビュー（CBV）の設定コマンド
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

    // 描画コマンドリスト
    iDX->GetCommandList()->DrawIndexedInstanced(_numIndices, 1, 0, 0, 0);
}