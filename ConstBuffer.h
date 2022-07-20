#pragma once
#include "Indispensable.h"

template <typename CBType> class ConstBuffer
{
    // �萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
    struct ConstBufferDataTransform {
        XMMATRIX mat;		// 3D�ϊ��s��
    };
public: // �����o�ϐ�
public: // �����o�֐�
    void Create();

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;

    // �萔�o�b�t�@�i�s��p�j
    Comptr<ID3D12Resource> ConstBuff = nullptr;

private: // �����o�֐�
};

