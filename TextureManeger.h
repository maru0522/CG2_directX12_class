#pragma once
#include "Indispensable.h"

struct Texture
{
    // ���O
    std::string name;
    // �e�N�X�`�����
    Microsoft::WRL::ComPtr<ID3D12Resource> buff = nullptr;
    // �n���h��
    D3D12_CPU_DESCRIPTOR_HANDLE srvCPUHandle{}; // CPU
    D3D12_GPU_DESCRIPTOR_HANDLE srvGPUHandle{}; // GPU
};

class TextureManeger
{
public: // �ÓI�����o�֐�
    void Load(std::string& fileName);

private: // �ÓI�����o�ϐ�
    // SRV�̍ő��
    static const size_t maxSRVDesc = 256;

public: // �����o�֐�
    TextureManeger();

    const Texture* GetTextureHandle(const std::string& fileName);

    // private�����o�ϐ�: srvHeap ���擾
    ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap.Get(); }

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

    const std::string directoryPath = "Resource/";

    // SRV�R���e�i
    std::array<Texture, maxSRVDesc> textures{};

    uint32_t indexNextDescHeap = 0;

};
// �C���X�^���X���擾
TextureManeger* GetInstanceTexM();

