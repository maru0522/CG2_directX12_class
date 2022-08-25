#pragma once
#include "Indispensable.h"

template <typename CBType> class ConstBuffer
{
public: // �����o�֐�
    ConstBuffer();
    ~ConstBuffer();

    // �}�e���A���J���[�̃Z�b�e�B���O
    CBType* GetCBTypeMap() { return &cbTypeMap }

    // private�����o�ϐ�: cBMaterial ���擾
    ID3D12Resource* GetBuffer() { return buffer.Get(); }

private: // �����o�ϐ�
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12Resource> buffer = nullptr;

    CBType* cbTypeMap = nullptr;

private: // �����o�֐�
    // cBMaterial��cMapMaterial�̃}�b�s���O
    void CBMap();

    // �r���ŐF��ω�������\�肪�Ȃ��Ȃ珉������Ɏg��
    // �}�b�s���O�̉���
    void CBUnMap();
};

