#include "Texture.h"
#include "InitDirectX.h"

size_t Texture::loadTexNum = 0;

Texture::Texture(std::string path)
{
    HRESULT result = S_FALSE;
    InitDirectX* iDX_ = GetInstanceIDX();

    std::wstring wStrPath(path.begin(), path.end());
    const wchar_t* szFile = wStrPath.c_str();

    //画像イメージデータ配列
    TexMetadata metadata{}; // 画像のサイズ等、テクスチャとしての各種情報が入る
    ScratchImage scratchImg{}; // ビットマップ形式での画像データそのもののアドレスが入る

    // WICテクスチャのロード
    result = LoadFromWICFile(szFile, WIC_FLAGS_NONE, &metadata, scratchImg);

    ScratchImage mipChain{};

    // ミップマップ生成
    result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(result)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }

    // 読み込んだディフューズテクスチャをSRGBとして扱う
    metadata.format = MakeSRGB(metadata.format);

    // ヒープ設定
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // リソース設定
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = static_cast<UINT>(metadata.height);
    textureResourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    textureResourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    textureResourceDesc.SampleDesc.Count = 1;

    // テクスチャバッファの生成
    result = iDX_->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&texBuff));
    assert(SUCCEEDED(result));

    // SRVヒープの先頭ハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = iDX_->GetSrvHeap()->GetCPUDescriptorHandleForHeapStart();

    // デスクリプタのサイズを取得する。
    UINT incrementSize = iDX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // 1枚目があるなら進める。3枚目以降は += incrementSize * 作成したSRVの数（テクスチャ）する必要がある。
    srvHandle.ptr += incrementSize * loadTexNum;

    // 全ミップマップについて
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // ミップマップレベルを指定してイメージを取得
        const Image* img = scratchImg.GetImage(i, 0, 0);

        // テクスチャバッファにデータ転送
        result = texBuff->WriteToSubresource(
            static_cast<UINT>(i),
            nullptr,		// 全領域へコピー
            img->pixels,	// 元データアドレス
            static_cast<UINT>(img->rowPitch),	// 1ラインサイズ
            static_cast<UINT>(img->slicePitch)	// 全サイズ
        );
        assert(SUCCEEDED(result));
    }

    // シェーダリソースビュー設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // ハンドルのさす位置にシェーダーリソースビューの作成
    iDX_->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

    loadTexNum++;
}