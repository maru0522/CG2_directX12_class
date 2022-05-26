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

#define DIRECTINPUT_VERSION 0x0800
class ObjTriangle
{
public:
	ObjTriangle(ID3D12Device* device, XMFLOAT3 vertices[3]);
	~ObjTriangle();

	void Draw(ID3D12GraphicsCommandList* commandList);

private:
	// ���_��
	static const int kVertNum = 3;
	// �C���f�b�N�X��
	static const int kIdxNum = 3;

#pragma region DirectX������
	//DirectX������
	HRESULT sResult;
	/*ID3D12Device* device;*/
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

	// ���_�f�[�^
	XMFLOAT3 vertices[kVertNum];

	// �C���f�b�N�X�f�[�^
	uint16_t indices[kIdxNum];

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB;

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};

	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff;

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	XMFLOAT3* vertMap;

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

#pragma endregion
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

	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB;

	// �C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff;

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap;

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
#pragma endregion
};