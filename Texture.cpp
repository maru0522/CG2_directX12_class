#include "Texture.h"
#include "InitDirectX.h"

size_t Texture::loadTexNum = 0;

Texture::Texture(std::string path)
{
    HRESULT result = S_FALSE;
    InitDirectX* iDX_ = GetInstanceIDX();

    std::wstring wStrPath(path.begin(), path.end());
    const wchar_t* szFile = wStrPath.c_str();

    //�摜�C���[�W�f�[�^�z��
    TexMetadata metadata{}; // �摜�̃T�C�Y���A�e�N�X�`���Ƃ��Ă̊e���񂪓���
    ScratchImage scratchImg{}; // �r�b�g�}�b�v�`���ł̉摜�f�[�^���̂��̂̃A�h���X������

    // WIC�e�N�X�`���̃��[�h
    result = LoadFromWICFile(szFile, WIC_FLAGS_NONE, &metadata, scratchImg);

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
        IID_PPV_ARGS(&texBuff));
    assert(SUCCEEDED(result));

    // SRV�q�[�v�̐擪�n���h�����擾
    D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = iDX_->GetSrvHeap()->GetCPUDescriptorHandleForHeapStart();

    // �f�X�N���v�^�̃T�C�Y���擾����B
    UINT incrementSize = iDX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // 1���ڂ�����Ȃ�i�߂�B3���ڈȍ~�� += incrementSize * �쐬����SRV�̐��i�e�N�X�`���j����K�v������B
    srvHandle.ptr += incrementSize * loadTexNum;

    // �S�~�b�v�}�b�v�ɂ���
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
        const Image* img = scratchImg.GetImage(i, 0, 0);

        // �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
        result = texBuff->WriteToSubresource(
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
    iDX_->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

    loadTexNum++;
}