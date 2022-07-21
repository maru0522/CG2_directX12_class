#include "Texture.h"
#include "InitDirectX.h"

namespace {
	// ワイド文字に変換
	std::wstring ConvertString(const std::string& text) {
		if (text.empty()) {
			return std::wstring();
		}

		/*
		 * ref: https://docs.microsoft.com/ja-JP/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
		 * ref: http://www.t-net.ne.jp/~cyfis/win_api/sdk/MultiByteToWideChar.html
		 * ref: http://chokuto.ifdef.jp/urawaza/api/MultiByteToWideChar.html
		 */
		int wcharNum =
			MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), NULL, 0);
		if (wcharNum == 0) {
			return std::wstring();
		}
		std::wstring result(wcharNum, 0);
		MultiByteToWideChar(
			CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), &result[0], wcharNum);
		return result;
	}
}

void Texture::LoadWICTex(std::string path)
{
	HRESULT result = S_FALSE;

	std::wstring szFile = ConvertString(path);

	// WICテクスチャのロード
	result = LoadFromWICFile(
		szFile.c_str(),   //「Resources」フォルダの「texture.png」
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	// ミップマップ生成
	result =
		GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャバッファの生成
	result = GetInstanceIDX()->GetDevice()->CreateCommittedResource(
		&textureHeapProp, D3D12_HEAP_FLAG_NONE, &textureResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texBuff));

	// 全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// ハンドルのどこから生成できるかを確認する
	UINT incrementSize = GetInstanceIDX()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// 生成してあるなら進める。（生成してないなら値には0が入るので進めない）
	srvHandle.ptr += incrementSize;


	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// ハンドルのさす位置にシェーダーリソースビューの作成
	GetInstanceIDX()->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
}