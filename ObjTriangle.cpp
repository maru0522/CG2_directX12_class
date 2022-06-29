#include "ObjTraiangle.h"

ObjTriangle::ObjTriangle(ID3D12Device* device, XMFLOAT3 vertices[3])
{
	// 頂点データ
	// インデックスデータ
	for (uint8_t i = 0; i < 3; i++) {
		this->vertices[i] = vertices[i];
		indices[i] = i;
	}

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(this->vertices));

	// 頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	vertBuff = nullptr;
	sResult = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(sResult));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	vertMap = nullptr;
	sResult = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(sResult));
	// 全頂点に対して
	for (int i = 0; i < _countof(this->vertices); i++) {
		vertMap[i] = this->vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);


	// 定数バッファ生成用の設定
	// ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;				// GPUへの転送用
	// リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	constBuffMaterial = nullptr;
	// 定数バッファの生成
	sResult = device->CreateCommittedResource(
		&cbHeapProp,	// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(sResult));

	// 定数バッファのマッピング
	constMapMaterial = nullptr;

	sResult = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);		// マッピング
	assert(SUCCEEDED(sResult));


	// 定数バッファ生成用の設定（3D行列変換）
	{
		// ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		// GPUへの転送
		// リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	// 256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 定数バッファの生成
		sResult = device->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(sResult));

		// 定数バッファのマッピング
		sResult = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
		assert(SUCCEEDED(sResult));
	}

	// 値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f);
	constMapTransform->mat = XMMatrixIdentity();

	// インデックスデータ全体のサイズ
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	// インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// インデックスバッファの生成
	indexBuff = nullptr;
	sResult = device->CreateCommittedResource(
		&heapProp,	// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// インデックスバッファをマッピング
	indexMap = nullptr;
	sResult = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// 全インデックスに対して
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];	// インデックスをコピー
	}
	// マッピング解除
	indexBuff->Unmap(0, nullptr);

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

ObjTriangle::~ObjTriangle()
{
}

void ObjTriangle::Draw(ID3D12GraphicsCommandList* commandList)
{
	// 頂点バッファビューの設定コマンド
	commandList->IASetVertexBuffers(0, 1, &vbView);

	// インデックスバッファビューの設定コマンド
	commandList->IASetIndexBuffer(&ibView);

	// 定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// 描画コマンド
	commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}