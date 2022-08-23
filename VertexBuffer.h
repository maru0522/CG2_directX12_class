#pragma once
#include "Indispensable.h"

// ���_�f�[�^�\����
struct Vertex
{
    XMFLOAT3 pos;       // xyz���W
    XMFLOAT3 normal;    // �@���x�N�g��
    XMFLOAT2 uv;        // uv���W
};

class VertexBuffer
{
public: // �����o�֐�
    VertexBuffer();

    // private�����o�ϐ�: vbView ���擾
    D3D12_VERTEX_BUFFER_VIEW* GetVbView() { return &vbView; }

private: // �����o�ϐ�
// �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12Resource> vertBuff = nullptr;

    // ���_�o�b�t�@�r���[�̍쐬
    D3D12_VERTEX_BUFFER_VIEW vbView{};
};

#pragma region ������
Vertex vertices[24] = {
    // �O
        {{-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// ���� 
        {{-5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}},		// ����
        {{ 5.0f, -5.0f, -5.0f }, {}, {1.0f, 1.0f}},		// �E��
        {{ 5.0f,  5.0f, -5.0f }, {}, {1.0f, 0.0f}},		// �E��
    // ��
        {{-5.0f, -5.0f,  5.0f }, {}, {0.0f, 1.0f}},		// ���� 
        {{-5.0f,  5.0f,  5.0f }, {}, {0.0f, 0.0f}},		// ����
        {{ 5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}},		// �E��
        {{ 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// �E��
    // ��
        {{-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// ���� 
        {{-5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}},		// ����
        {{-5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}},		// �E��
        {{-5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// �E��
    // �E
        {{ 5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// ���� 
        {{ 5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}},		// ����
        {{ 5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}},		// �E��
        {{ 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// �E��
    // ��
        {{-5.0f,  5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// ���� 
        {{ 5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}},		// ����
        {{-5.0f,  5.0f,  5.0f }, {}, {1.0f, 1.0f}},		// �E��
        {{ 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// �E��
    // ��
        {{-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// ���� 
        {{ 5.0f, -5.0f, -5.0f }, {}, {0.0f, 0.0f}},		// ����
        {{-5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}},		// �E��
        {{ 5.0f, -5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// �E��
};
#pragma endregion