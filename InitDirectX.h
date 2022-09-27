#pragma once
#include <d3d12.h>      // CG2_01_02   7����
#include <dxgi1_6.h>    // CG2_01_02   7����
#include <vector>       // CG2_01_02   12����
#include <wrl.h>        // CG2_07_01   13����

class InitDirectX
{
public: // �ÓI�֐�
    static InitDirectX* GetInstance(); // �C���X�^���X���擾

public: // �֐�
    void Initialize();
    void PreDraw();
    void PostDraw();

    // private�����o�ϐ�: device ���擾
    ID3D12Device* GetDevice() { return device.Get(); }

    // private�����o�ϐ�: commandList ���擾
    ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }
private: // �֐�
    void DebugLayer();      // DXGIDevice() �Ŏg�p
    void DXGIDevice();      // �A�_�v�^�̑I��/�f�o�C�X�̐���
    void Command();         // �R�}���h�A���P�[�^/���X�g/�L���[�̐���
    void SwapChain();
    void RTVDescHeap();     // RTV�p�f�X�N���v�^�q�[�v�̐��� + �������邽�߂̐ݒ�i�f�X�N���v�^�j
    void RTV();             // RTV�̐��� + �������邽�߂̐ݒ�i�f�X�N���v�^�j
    void DepthBuffer();
    void Fence();

    void ClearRTV();        // RTV���N���A
    void ClearDepthBuff();  // �[�x�o�b�t�@���N���A

private: // �ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // DebugLayer() �ŏ�����
    ComPtr<ID3D12Debug> debugController; // DXGIDevice() �Ŏg�p

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
};