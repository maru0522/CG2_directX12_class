#include "Texture.h"
#include "InitDirectX.h"

void Texture::ClearImgData(XMFLOAT4* _imgData)
{
    // �S�s�N�Z���̐F��������
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

	// �摜�C���[�W�f�[�^�z��
	XMFLOAT4* imgData = new XMFLOAT4[imgDataCount];		// ���K����ŉ������
	ClearImgData(imgData);

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = texWidth;	// ��
	textureResourceDesc.Height = texHeight;	// ����
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

	// �e�N�X�`���o�b�t�@�Ƀf�[�^����
	result = texBuff->WriteToSubresource(
		0,
		nullptr,	// �S�̈�փR�s�[
		imgData,	// ���f�[�^�A�h���X
		sizeof(XMFLOAT4) * texWidth,	// 1���C���T�C�Y
		sizeof(XMFLOAT4) * imgDataCount	// �S�T�C�Y
	);

	// ���f�[�^���
	delete[] imgData;
}

void Texture::CreateSrvHeapDesc()
{
	HRESULT result = S_FALSE;

	// SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// �ݒ������SRV�p�f�X�N���v�^�q�[�v���쐬
	result = GetInstanceIDX()->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
}

void Texture::CreateSrv()
{
	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// �ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	// RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	// �n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�̍쐬
	GetInstanceIDX()->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);
}