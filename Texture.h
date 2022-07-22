#pragma once
#include "Indispensable.h"

class Texture
{
public: // �����o�֐�
    void LoadWICTex(std::string path);

    UINT GetIncrementSize() { return incrementSize; }

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    // �摜�C���[�W�f�[�^�z��
    TexMetadata metadata{}; // �摜�̃T�C�Y���A�e�N�X�`���Ƃ��Ă̊e���񂪓���
    ScratchImage scratchImg{}; // �r�b�g�}�b�v�`���ł̉摜�f�[�^���̂��̂̃A�h���X������

    Comptr<ID3D12Resource> texBuff = nullptr;

    UINT incrementSize;
};

