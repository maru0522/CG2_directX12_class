#pragma once
#include "Indispensable.h"

// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
struct ConstBufferDataMaterial {
    XMFLOAT4 color;		// �F�iRGBA�j
};

class ConstBuffer
{
public: // �����o�֐�
    ConstBuffer();

    // �}�e���A���J���[�̃Z�b�e�B���O
    void SetColor(float R, float G, float B, float A) { cMapMaterial->color = XMFLOAT4(R, G, B, A); }

    // private�����o�ϐ�: cBMaterial ���擾
    ID3D12Resource* GetCBMaterial() { return cBMaterial.Get(); }

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12Resource> cBMaterial = nullptr;
    ConstBufferDataMaterial* cMapMaterial = nullptr;

private: // �����o�֐�
    // cBMaterial��cMapMaterial�̃}�b�s���O
    void CBMap();

    // �r���ŐF��ω�������\�肪�Ȃ��Ȃ珉������Ɏg��
    // �}�b�s���O�̉���
    void CBUnMap();
};

