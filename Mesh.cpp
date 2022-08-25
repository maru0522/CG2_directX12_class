#include "Mesh.h"
#include "InitDirectX.h"

Mesh::Mesh()
{
    HRESULT result = S_FALSE;

    InitDirectX* iDX_ = GetInstanceIDX();

    // ���_�f�[�^
    Vertex vertices[] = {
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

    // �C���f�b�N�X�f�[�^
    unsigned short indices[] =
    {
        // �O
            0,1,2,      //�O�p�`1��
            2,1,3,      //�O�p�`2��
        // ��
            4,6,5,      //�O�p�`3��
            6,7,5,      //�O�p�`4��
        // ��
            8,9,10,     //�O�p�`5��
            10,9,11,    //�O�p�`6��
        // �E
            12,14,13,   //�O�p�`7��
            14,15,13,   //�O�p�`8��
        // ��
            16,18,17,   //�O�p�`9��
            18,19,17,   //�O�p�`10��
        // ��
            20,21,22,   //�O�p�`11��
            22,21,23,   //�O�p�`12��
    };

    for (int i = 0; i < _countof(indices) / 3; i++) {
        // �O�p�`1���ƂɌv�Z���Ă���
        // �O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
        unsigned short index0 = indices[i * 3 + 0];
        unsigned short index1 = indices[i * 3 + 1];
        unsigned short index2 = indices[i * 3 + 2];
        // �O�p�`���\�����钸�_���W���x�N�g���ɑ��
        XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
        XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
        XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
        // p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
        XMVECTOR v1 = XMVectorSubtract(p1, p0);
        XMVECTOR v2 = XMVectorSubtract(p2, p0);
        // �O�ς͗������琂���ȃx�N�g��
        XMVECTOR normal = XMVector3Cross(v1, v2);
        // ���K���i������1�ɂ���j
        normal = XMVector3Normalize(normal);
        // ���߂��@���𒸓_�f�[�^�ɑ��
        XMStoreFloat3(&vertices[index0].normal, normal);
        XMStoreFloat3(&vertices[index1].normal, normal);
        XMStoreFloat3(&vertices[index2].normal, normal);
    }

#pragma region vb
    // ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
    UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(vertices));

    // ���_�o�b�t�@�̐ݒ�
    D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // ���_�o�b�t�@�̐���
    result = iDX_->GetDevice()->CreateCommittedResource(
        &heapProp, // �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(result));

    // GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
    Vertex* vertMap = nullptr;
    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));

    // �S���_�ɑ΂���
    for (int i = 0; i < _countof(vertices); i++) {
        vertMap[i] = vertices[i];//���W���R�s�[
    }

    // �q���������
    vertBuff->Unmap(0, nullptr);

    // ���_�o�b�t�@�r���[�̍쐬
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress(); // GPU���z�A�h���X
    vbView.SizeInBytes = sizeVB; // ���_�o�b�t�@�̃T�C�Y
    vbView.StrideInBytes = sizeof(vertices[0]); // ���_1���̃f�[�^�T�C�Y
#pragma endregion

#pragma region ib
    // �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
    UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

    // ���\�[�X�ݒ�
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeIB;	// �C���f�b�N�X��񂪓��镪�̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // �C���f�b�N�X�o�b�t�@�̐���
    result = iDX_->GetDevice()->CreateCommittedResource(
        &heapProp,	// �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc,	// ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuff));

    // �C���f�b�N�X�o�b�t�@���}�b�s���O
    uint16_t* indexMap = nullptr;
    result = indexBuff->Map(0, nullptr, (void**)&indexMap);

    // �S�C���f�b�N�X�ɑ΂���
    for (int i = 0; i < _countof(indices); i++) {
        indexMap[i] = indices[i];	// �C���f�b�N�X���R�s�[
    }

    // �}�b�s���O����
    indexBuff->Unmap(0, nullptr);

    // �C���f�b�N�X�o�b�t�@�r���[�̍쐬
    ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R16_UINT;
    ibView.SizeInBytes = sizeIB;
#pragma endregion
}