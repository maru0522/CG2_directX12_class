#include "Texture.h"
#include "InitDirectX.h"

void Texture::ClearImgData(XMFLOAT4* _imgData)
{
    // 全ピクセルの色を初期化
    for (size_t i = 0; i < imgDataCount; i++) {
    	_imgData[i].x = 1.0f;		//R
    	_imgData[i].y = 0.0f;		//G
    	_imgData[i].z = 0.0f;		//B
    	_imgData[i].w = 1.0f;		//A
    }
}

void Texture::CreateTexBuff()
{
	HRESULT result = S_FALSE;

	// 画像イメージデータ配列
	XMFLOAT4* imgData = new XMFLOAT4[imgDataCount];		// ※必ず後で解放する
	ClearImgData(imgData);

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = texWidth;	// 幅
	textureResourceDesc.Height = texHeight;	// 高さ
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;

	result = GetInstanceIDX()->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	// テクスチャバッファにデータ生成
	result = texBuff->WriteToSubresource(
		0,
		nullptr,	// 全領域へコピー
		imgData,	// 元データアドレス
		sizeof(XMFLOAT4) * texWidth,	// 1ラインサイズ
		sizeof(XMFLOAT4) * imgDataCount	// 全サイズ
	);

	// 元データ解放
	delete[] imgData;
}

void Texture::CreateSrvHeapDesc()
{
	HRESULT result = S_FALSE;

	// SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	// デスクリプタヒープの設定
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// 設定を元にSRV用デスクリプタヒープを作成
	result = GetInstanceIDX()->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
}

void Texture::CreateSrv()
{
	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// 設定構造体
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	// RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	// ハンドルのさす位置にシェーダーリソースビューの作成
	GetInstanceIDX()->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
}