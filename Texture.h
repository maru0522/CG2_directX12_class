#pragma once
#include "Indispensable.h"
#include "InitDirectX.h"

//class Texture
//{
//public: // �ÓI�����o�֐�
//    // private�����o�ϐ�: loadTexNum ���擾
//    static size_t GetLoadTexNum() { return loadTexNum; }
//
//private: // �ÓI�����o�ϐ�
//    // �ǂݍ��܂ꂽ�e�N�X�`���̐��i= �������ꂽSRV�̐��j
//    static size_t loadTexNum;
//
//public: // �����o�֐�
//    Texture(std::string path);
//
//    // private�����o�ϐ�: srvCPUHandle ���擾
//    D3D12_CPU_DESCRIPTOR_HANDLE* GetSrvCpuHandle() { return &srvCPUHandle; }
//    // private�����o�ϐ�: srvGPUHandle ���擾
//    D3D12_GPU_DESCRIPTOR_HANDLE* GetSrvGpuHandle() { return &srvGPUHandle; }
//
//private: // �����o�ϐ�
//    // �G�C���A�X�e���v���[�g
//    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//
//    ComPtr<ID3D12Resource> texBuff = nullptr;
//
//    D3D12_CPU_DESCRIPTOR_HANDLE srvCPUHandle{}; // CPU
//    D3D12_GPU_DESCRIPTOR_HANDLE srvGPUHandle{}; // GPU
//};
