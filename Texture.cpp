#include "Texture.h"
#include "InitDirectX.h"

namespace {
	// ���C�h�����ɕϊ�
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

	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		szFile.c_str(),   //�uResources�v�t�H���_�́utexture.png�v
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	// �~�b�v�}�b�v����
	result =
		GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���o�b�t�@�̐���
	result = GetInstanceIDX()->GetDevice()->CreateCommittedResource(
		&textureHeapProp, D3D12_HEAP_FLAG_NONE, &textureResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texBuff));

	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // �S�̈�փR�s�[
			img->pixels,          // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// �n���h���̂ǂ����琶���ł��邩���m�F����
	UINT incrementSize = GetInstanceIDX()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// �������Ă���Ȃ�i�߂�B�i�������ĂȂ��Ȃ�l�ɂ�0������̂Ői�߂Ȃ��j
	srvHandle.ptr += incrementSize;


	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// �n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�̍쐬
	GetInstanceIDX()->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
}