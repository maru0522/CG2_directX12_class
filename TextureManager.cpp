#include "TextureManager.h"
#include "InitDirectX.h"

void TextureManager::Load(const std::string& fileName)
{
    // SRVDescに空きがあるかどうか
    assert(("Message: SRVがDescの最大数に達しました。。これ以上テクスチャを読み込みません。", indexNextDescHeap < maxSRVDesc));
    uint32_t handle = indexNextDescHeap;

    // 読み込もうとしてるテクスチャ（の名前）が既に読み込まれているかどうか検索
    auto iterator = std::find_if(textures.begin(), textures.end(), [&](const auto& texture) { return texture.name == fileName; });

    // テクスチャの重複が見つかった場合
    if (iterator != textures.end()) {
        // 同じテクスチャが入っているSRVの番号を取得し代入する
        handle = static_cast<uint32_t>(std::distance(textures.begin(), iterator));
    }

    // 書き込むテクスチャを参照渡し
    Texture& texMirror_ = textures.at(handle);
    texMirror_.name = fileName;

    std::string relativePath = directoryPath + fileName;
    std::wstring wRelativePath(relativePath.begin(), relativePath.end());
    const wchar_t* szFile_ = wRelativePath.c_str();

    HRESULT result = S_FALSE;
    InitDirectX* iDX_ = GetInstanceIDX();
    TextureManager* texM_ = GetInstanceTexM();

    //画像イメージデータ配列
    TexMetadata metadata{}; // 画像のサイズ等、テクスチャとしての各種情報が入る
    ScratchImage scratchImg{}; // ビットマップ形式での画像データそのもののアドレスが入る

    // WICテクスチャのロード
    result = LoadFromWICFile(szFile_, WIC_FLAGS_NONE, &metadata, scratchImg);

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
        IID_PPV_ARGS(&texMirror_.buff));
    assert(SUCCEEDED(result));

    // SRVヒープの先頭ハンドルを取得
    texMirror_.srvCPUHandle = texM_->GetSrvHeap()->GetCPUDescriptorHandleForHeapStart();

    // デスクリプタのサイズを取得する。
    UINT incrementSize = iDX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // 1枚目があるなら進める。3枚目以降は += incrementSize * 作成したSRVの数（テクスチャ）する必要がある。
    texMirror_.srvCPUHandle.ptr += incrementSize * handle;

    // 全ミップマップについて
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // ミップマップレベルを指定してイメージを取得
        const Image* img = scratchImg.GetImage(i, 0, 0);

        // テクスチャバッファにデータ転送
        result = texMirror_.buff->WriteToSubresource(
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
    iDX_->GetDevice()->CreateShaderResourceView(texMirror_.buff.Get(), &srvDesc, texMirror_.srvCPUHandle);

    texMirror_.srvGPUHandle = texM_->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
    texMirror_.srvCPUHandle.ptr += incrementSize * handle;

    indexNextDescHeap++;
}

void TextureManager::LoadTexture(const std::string& fileName)
{
    texM.Load(fileName);
}

TextureManager::TextureManager()
{
    HRESULT result = S_FALSE;

    InitDirectX* iDX_ = GetInstanceIDX();

    // デスクリプタヒープの設定
    D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
    srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダから見えるように
    srvDescHeapDesc.NumDescriptors = maxSRVDesc;

    // 設定を元にSRV用デスクリプタヒープを作成
    result = iDX_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&srvHeap));
    assert(SUCCEEDED(result));
}

const Texture* TextureManager::GetTextureHandle(const std::string& fileName)
{
    // 同じファイル名のテクスチャを検索
    auto iterator = std::find_if(textures.begin(), textures.end(), [&](const auto& texture) { return texture.name == fileName; });

    // 見つからんかった場合
    assert(("Message: 指定されたテクスチャは見つかりませんでした。", iterator != textures.end()));

    uint32_t handle = 0;

    // 見つかった場合
    if (iterator != textures.end()) {
        // 同じテクスチャが入っているSRVの番号を取得し代入する
        handle = static_cast<uint32_t>(std::distance(textures.begin(), iterator));
    }

    // 書き込むテクスチャを参照渡し
    Texture& texMirror_ = textures.at(handle);

    return &texMirror_;
}

static TextureManager texM;
TextureManager* GetInstanceTexM()
{
    return &texM;
}
