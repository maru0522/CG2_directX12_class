#pragma once
#include "Indispensable.h"

class InitDirectX
{
public: // �����o�֐�
    void Initialize(); // DirectX����������

    void PreDraw(); // �`��O����

    void PostDraw(); // �`��㏈��

    void ClearRTV(); // RTV�̃N���A

    void ClearDepthBuffer(); // �[�x�o�b�t�@�̃N���A

    ID3D12Device* GetDevice() { return device.Get(); } // private�����o�ϐ�: device ���擾

    ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); } // private�����o�ϐ�: commandList ���擾

    ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap.Get(); } // private�����o�ϐ�: srvHeap ���擾

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    Comptr<IDXGIFactory7> dxgiFactory = nullptr;
    Comptr<ID3D12Device> device = nullptr;
    Comptr<ID3D12CommandAllocator> cmdAllocator = nullptr;
    Comptr<ID3D12GraphicsCommandList> commandList = nullptr;
    Comptr<ID3D12CommandQueue> commandQueue = nullptr;
    Comptr<IDXGISwapChain4> swapChain = nullptr;
    Comptr<ID3D12DescriptorHeap> srvHeap = nullptr;
    Comptr<ID3D12DescriptorHeap> rtvHeap = nullptr;
    std::vector<Comptr<ID3D12Resource>> backBuffers;
    Comptr<ID3D12Resource> depthBuff = nullptr;
    Comptr<ID3D12DescriptorHeap> dsvHeap = nullptr;
    Comptr<ID3D12Fence> fence = nullptr;
    UINT64 fenceVal = 0;

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
    D3D12_RESOURCE_BARRIER barrierDesc{};

private: // �����o�֐�

    // �f�o�C�X�̐���
    void DXGIDevice();
    // �R�}���h�A���P�[�^/���X�g/�L���[�̐���
    void Command();
    // �X���b�v�`�F�[���̐���
    void SwapChain();
    // SRVDescHeap�̐���
    void SRVDescHeap();
    // RTVDescHeap�̐���
    void RTVDescHeap();
    // �[�x�o�b�t�@�̐���
    void DepthBuffer();
    // �t�F���X�̐���
    void Fence();
};

// �C���X�^���X���擾
InitDirectX* GetInstanceIDX();
