#pragma once
#include "Indispensable.h"

class Texture
{
public:
public:
    void LoadWICTex(std::string path);

private:
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    // �摜�C���[�W�f�[�^�z��
    TexMetadata metadata{}; // �摜�̃T�C�Y���A�e�N�X�`���Ƃ��Ă̊e���񂪓���
    ScratchImage scratchImg{}; // �r�b�g�}�b�v�`���ł̉摜�f�[�^���̂��̂̃A�h���X������

    Comptr<ID3D12Resource> texBuff = nullptr;
    Comptr<ID3D12DescriptorHeap> srvHeap = nullptr;

private:
};

