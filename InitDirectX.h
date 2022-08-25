#pragma once
#include "Indispensable.h"
#include "Window.h"

class InitDirectX
{
public: // �����o�֐�
    void Initialize();
    void PreDraw();
    void PostDraw();

    // private�����o�ϐ�: device ���擾
    ID3D12Device* GetDevice() { return device.Get(); }

    // private�����o�ϐ�: commandList ���擾
    ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // DXGIDevice() �ŏ�����
    ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
    ComPtr<ID3D12Device> device = nullptr;

    // Coomand() �ŏ�����
    ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
    ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
    ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
 
    // SwapChain() �ŏ�����
    ComPtr<IDXGISwapChain4> swapChain = nullptr;
    
    // RtvDescHeap() �ŏ�����
    ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};

    // Rtv() �ŏ�����
    std::vector<ComPtr<ID3D12Resource>> backBuffers; // �o�b�N�o�b�t�@

    // DepthBuffer() �ŏ�����
    ComPtr<ID3D12Resource> depthBuff = nullptr;
    ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

    // Fence() �ŏ�����
    ComPtr<ID3D12Fence> fence = nullptr;
    UINT64 fenceVal = 0;

private: // �����o�֐�
    void DXGIDevice(); // �A�_�v�^�̑I��/�f�o�C�X�̐���
    void Command(); // �R�}���h�A���P�[�^/���X�g/�L���[�̐���
    void SwapChain();
    void RtvDescHeap(); // RTV�p�̃f�X�N���v�^�q�[�v�̐��� + �������邽�߂̐ݒ�i�f�X�N���v�^�j
    // void Srv(); // SRV�̐��� + �������邽�߂̐ݒ�i�f�X�N���v�^�j
    void Rtv(); // RTV�̐��� + �������邽�߂̐ݒ�i�f�X�N���v�^�j
    void DepthBuffer();
    void Fence();

    void ClearRTV(); // RTV���N���A
    void ClearDepthBuff(); // �[�x�o�b�t�@���N���A
};
// �C���X�^���X���擾
InitDirectX* GetInstanceIDX();

