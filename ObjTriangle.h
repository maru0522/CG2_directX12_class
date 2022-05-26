#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include<d3d12.h>
#include<dxgi1_6.h>
#include<d3dcompiler.h>
#include<cassert>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<vector>
#include<string>
#include<DirectXTex.h>

#define DIRECTINPUT_VERSION 0x0800
class ObjTriangle
{
public:
	ObjTriangle();
	~ObjTriangle();

	void Initialize();
	void Update();

private:
	// ���_��
	static const int kVertNum = 4;
	// �C���f�b�N�X��
	static const int kIdxNum = 6;

#pragma region DirectX������
	//DirectX������
	HRESULT sResult;
	ID3D12Device* sDevice;
	/*IDXGIFactory7* dxgiFactory;
	IDXGISwapChain4* swapChain;
	ID3D12CommandAllocator* cmdAllocator;
	ID3D12GraphicsCommandList* commandList;
	ID3D12CommandQueue* commandQueue;
	ID3D12DescriptorHeap* rtvHeap;*/

#pragma region �A�_�v�^�̗�
	// �I�u�W�F�N�g�𑝂₷���ƂɃA�_�v�^��񋓂��鏈���͔�������ƍl���A�N���X�ł̃A�_�v�^�̗񋓂͂��Ȃ��B
	/*std::vector<IDXGIAdapter4*> adapters;*/
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	/*IDXGIAdapter4* tmpAdapter;*/
#pragma endregion

#pragma region �f�o�C�X�̐���
	// �I�u�W�F�N�g�𑝂₷���ƂɃf�o�C�X�𐶐����鏈���͔�������ƍl���A�N���X�ł̃f�o�C�X�̐����͂��Ȃ��B
	// �Ή����x���̔z��
	/*D3D_FEATURE_LEVEL levels[4];*/

	/*D3D_FEATURE_LEVEL featureLevel;*/
#pragma endregion

#pragma region �R�}���h�L���[�̐���
	// �I�u�W�F�N�g�𑝂₷���ƂɎ��s���߂������Ă��������͔�������ƍl���A�N���X�ł̃L���[�̐����͂��Ȃ��B
	//�R�}���h�L���[�̐ݒ�
	/*D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};*/
#pragma endregion

#pragma region �X���b�v�`�F�[���̐���
	// �I�u�W�F�N�g�𑝂₷���Ƃ�2�̃o�b�t�@�[���̃��������m�ۂ���͔̂�������ƍl���A�N���X�ł̃X���b�v�`�F�[���̐����͂��Ȃ��B
	// �X���b�v�`�F�[���̐ݒ�
	/*DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};*/
#pragma endregion

#pragma region �����_�[�r���[�^�[�Q�b�g
	// �I�u�W�F�N�g�𑝂₷���ƂɃo�b�t�@�̏������������������Ă����͔̂�������ƍl���A�N���X�ł̃����_�[�^�[�Q�b�g�r���[�̐����͂��Ȃ��B
	// �f�X�N���v�^�q�[�v�̐ݒ�
	/*D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};*/

	// SRV�̍ő��
	/*const size_t kMaxSRVCount;*/

	// �f�X�N���v�^�q�[�v�̐ݒ�
	/*D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};*/

	// �ݒ������SRV�p�f�X�N���v�^�q�[�v���쐬
	/*ID3D12DescriptorHeap* srvHeap;*/

	// SRV�q�[�v�̐擪�n���h�����擾
	/*D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;*/

	// �o�b�N�o�b�t�@
	/*std::vector<ID3D12Resource*> backBuffers;*/
#pragma endregion

#pragma region �t�F���X
	// �I�u�W�F�N�g�𑝂₷���Ƃ�GPU�̏������������������Ď����鏈���������Ă����͔̂�������ƍl���A�N���X�ł̃t�F���X�����͂��Ȃ��B
	// �t�F���X�̐���
	/*ID3D12Fence* fence;*/
	/*UINT64 fenceVal;*/
#pragma endregion
#pragma endregion

#pragma region �`�揉���������ϐ�
	// ���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;	// xyz���W
		XMFLOAT2 uv;	// uv���W
	};

	// ���_�f�[�^
	Vertex vertices[kVertNum];

	// �C���f�b�N�X�f�[�^
	unsigned short indices[kIdxNum];

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB;

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};

	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff;

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap;

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

#pragma region ���_�V�F�[�_�[�ϐ�
	ID3DBlob* vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob; // �G���[�I�u�W�F�N�g
#pragma endregion

#pragma region ���_���C�A�E�g
// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
#pragma region �u�����h�X�e�[�g
	// --�R���X�g���N�^�ŃG���[���o�邽�ߎd���Ȃ��������B
	// --142�s�ڂ� pipelineDesc���������q�ŏ��������Ă��邽�߂����炭0������͂��B
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
#pragma endregion
	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	// ���[�g�p�����[�^�[�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[2] = {};

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3D10Blob* rootSigBlob;
#pragma endregion

#pragma region �p�C�v���C���X�e�[�g�̐���
	// �p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState;
#pragma endregion

	struct ConstBufferDataMaterial {
		XMFLOAT4 color;		// �F�iRGBA�j
	};

	// �萔�o�b�t�@�����p�̐ݒ�
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};

	// �萔�o�b�t�@�̐���
	ID3D12Resource* constBuffMaterial;

	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial;

#pragma region �e�N�X�`���}�b�s���O
	// �������s�N�Z����
	const size_t textureWidth = 256;
	// �c�����s�N�Z����
	const size_t textureHeight = 256;
	// �z��̗v�f��
	const size_t imageDataCount = textureWidth * textureHeight;

	//�摜�C���[�W�f�[�^�z��
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	ScratchImage mipChain{};

	// �摜�C���[�W�f�[�^�z��
	XMFLOAT4* imageData;

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};

	// �e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff;

	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
#pragma endregion

	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB;

	// �C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff;

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap;

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
#pragma endregion

#pragma region �O���t�B�b�N�X�R�}���h
	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	// �V�U�[��`
	D3D12_RECT scissorRect{};
#pragma endregion
};
