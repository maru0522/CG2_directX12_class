#include "TextureManager.h"
#include "InitDirectX.h"

void TextureManager::Load(const std::string& fileName)
{
    // SRVDesc�ɋ󂫂����邩�ǂ���
    assert(("Message: SRV��Desc�̍ő吔�ɒB���܂����B�B����ȏ�e�N�X�`����ǂݍ��݂܂���B", indexNextDescHeap < maxSRVDesc));
    uint32_t handle = indexNextDescHeap;

    // �ǂݍ������Ƃ��Ă�e�N�X�`���i�̖��O�j�����ɓǂݍ��܂�Ă��邩�ǂ�������
    auto iterator = std::find_if(textures.begin(), textures.end(), [&](const auto& texture) { return texture.name == fileName; });

    // �e�N�X�`���̏d�������������ꍇ
    if (iterator != textures.end()) {
        // �����e�N�X�`���������Ă���SRV�̔ԍ����擾���������
        handle = static_cast<uint32_t>(std::distance(textures.begin(), iterator));
    }

    // �������ރe�N�X�`�����Q�Ɠn��
    Texture& texMirror_ = textures.at(handle);
    texMirror_.name = fileName;

    std::string relativePath = directoryPath + fileName;
    std::wstring wRelativePath(relativePath.begin(), relativePath.end());
    const wchar_t* szFile_ = wRelativePath.c_str();

    HRESULT result = S_FALSE;
    InitDirectX* iDX_ = GetInstanceIDX();
    TextureManager* texM_ = GetInstanceTexM();

    //�摜�C���[�W�f�[�^�z��
    TexMetadata metadata{}; // �摜�̃T�C�Y���A�e�N�X�`���Ƃ��Ă̊e���񂪓���
    ScratchImage scratchImg{}; // �r�b�g�}�b�v�`���ł̉摜�f�[�^���̂��̂̃A�h���X������

    // WIC�e�N�X�`���̃��[�h
    result = LoadFromWICFile(szFile_, WIC_FLAGS_NONE, &metadata, scratchImg);

    ScratchImage mipChain{};

    // �~�b�v�}�b�v����
    result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(result)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }

    // �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
    metadata.format = MakeSRGB(metadata.format);

    // �q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = static_cast<UINT>(metadata.height);
    textureResourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    textureResourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    textureResourceDesc.SampleDesc.Count = 1;

    // �e�N�X�`���o�b�t�@�̐���
    result = iDX_->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&texMirror_.buff));
    assert(SUCCEEDED(result));

    // SRV�q�[�v�̐擪�n���h�����擾
    texMirror_.srvCPUHandle = texM_->GetSrvHeap()->GetCPUDescriptorHandleForHeapStart();

    // �f�X�N���v�^�̃T�C�Y���擾����B
    UINT incrementSize = iDX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // 1���ڂ�����Ȃ�i�߂�B3���ڈȍ~�� += incrementSize * �쐬����SRV�̐��i�e�N�X�`���j����K�v������B
    texMirror_.srvCPUHandle.ptr += incrementSize * handle;

    // �S�~�b�v�}�b�v�ɂ���
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
        const Image* img = scratchImg.GetImage(i, 0, 0);

        // �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
        result = texMirror_.buff->WriteToSubresource(
            static_cast<UINT>(i),
            nullptr,		// �S�̈�փR�s�[
            img->pixels,	// ���f�[�^�A�h���X
            static_cast<UINT>(img->rowPitch),	// 1���C���T�C�Y
            static_cast<UINT>(img->slicePitch)	// �S�T�C�Y
        );
        assert(SUCCEEDED(result));
    }

    // �V�F�[�_���\�[�X�r���[�ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // �n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�̍쐬
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

    // �f�X�N���v�^�q�[�v�̐ݒ�
    D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
    srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
    srvDescHeapDesc.NumDescriptors = maxSRVDesc;

    // �ݒ������SRV�p�f�X�N���v�^�q�[�v���쐬
    result = iDX_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&srvHeap));
    assert(SUCCEEDED(result));
}

const Texture* TextureManager::GetTextureHandle(const std::string& fileName)
{
    // �����t�@�C�����̃e�N�X�`��������
    auto iterator = std::find_if(textures.begin(), textures.end(), [&](const auto& texture) { return texture.name == fileName; });

    // ������񂩂����ꍇ
    assert(("Message: �w�肳�ꂽ�e�N�X�`���͌�����܂���ł����B", iterator != textures.end()));

    uint32_t handle = 0;

    // ���������ꍇ
    if (iterator != textures.end()) {
        // �����e�N�X�`���������Ă���SRV�̔ԍ����擾���������
        handle = static_cast<uint32_t>(std::distance(textures.begin(), iterator));
    }

    // �������ރe�N�X�`�����Q�Ɠn��
    Texture& texMirror_ = textures.at(handle);

    return &texMirror_;
}

static TextureManager texM;
TextureManager* GetInstanceTexM()
{
    return &texM;
}
