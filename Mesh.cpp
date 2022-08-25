#include "Mesh.h"
#include "InitDirectX.h"

Mesh::Mesh()
{
    HRESULT result = S_FALSE;

    InitDirectX* iDX_ = GetInstanceIDX();

    // 頂点データ
    Vertex vertices[] = {
        // 前
            {{-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
            {{-5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}},		// 左上
            {{ 5.0f, -5.0f, -5.0f }, {}, {1.0f, 1.0f}},		// 右下
            {{ 5.0f,  5.0f, -5.0f }, {}, {1.0f, 0.0f}},		// 右上
        // 後
            {{-5.0f, -5.0f,  5.0f }, {}, {0.0f, 1.0f}},		// 左下 
            {{-5.0f,  5.0f,  5.0f }, {}, {0.0f, 0.0f}},		// 左上
            {{ 5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}},		// 右下
            {{ 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
        // 左
            {{-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
            {{-5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}},		// 左上
            {{-5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}},		// 右下
            {{-5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
        // 右
            {{ 5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
            {{ 5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f}},		// 左上
            {{ 5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f}},		// 右下
            {{ 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
        // 上
            {{-5.0f,  5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
            {{ 5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f}},		// 左上
            {{-5.0f,  5.0f,  5.0f }, {}, {1.0f, 1.0f}},		// 右下
            {{ 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
        // 下
            {{-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f}},		// 左下 
            {{ 5.0f, -5.0f, -5.0f }, {}, {0.0f, 0.0f}},		// 左上
            {{-5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f}},		// 右下
            {{ 5.0f, -5.0f,  5.0f }, {}, {1.0f, 0.0f}},		// 右上
    };

    // インデックスデータ
    unsigned short indices[] =
    {
        // 前
            0,1,2,      //三角形1つ目
            2,1,3,      //三角形2つ目
        // 後
            4,6,5,      //三角形3つ目
            6,7,5,      //三角形4つ目
        // 左
            8,9,10,     //三角形5つ目
            10,9,11,    //三角形6つ目
        // 右
            12,14,13,   //三角形7つ目
            14,15,13,   //三角形8つ目
        // 上
            16,18,17,   //三角形9つ目
            18,19,17,   //三角形10つ目
        // 下
            20,21,22,   //三角形11つ目
            22,21,23,   //三角形12つ目
    };

    for (int i = 0; i < _countof(indices) / 3; i++) {
        // 三角形1つごとに計算していく
        // 三角形のインデックスを取り出して、一時的な変数に入れる
        unsigned short index0 = indices[i * 3 + 0];
        unsigned short index1 = indices[i * 3 + 1];
        unsigned short index2 = indices[i * 3 + 2];
        // 三角形を構成する頂点座標をベクトルに代入
        XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
        XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
        XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
        // p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
        XMVECTOR v1 = XMVectorSubtract(p1, p0);
        XMVECTOR v2 = XMVectorSubtract(p2, p0);
        // 外積は両方から垂直なベクトル
        XMVECTOR normal = XMVector3Cross(v1, v2);
        // 正規化（長さを1にする）
        normal = XMVector3Normalize(normal);
        // 求めた法線を頂点データに代入
        XMStoreFloat3(&vertices[index0].normal, normal);
        XMStoreFloat3(&vertices[index1].normal, normal);
        XMStoreFloat3(&vertices[index2].normal, normal);
    }

#pragma region vb
    // 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
    UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(vertices));

    // 頂点バッファの設定
    D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

    // リソース設定
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // 頂点データ全体のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 頂点バッファの生成
    result = iDX_->GetDevice()->CreateCommittedResource(
        &heapProp, // ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(result));

    // GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
    Vertex* vertMap = nullptr;
    result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    assert(SUCCEEDED(result));

    // 全頂点に対して
    for (int i = 0; i < _countof(vertices); i++) {
        vertMap[i] = vertices[i];//座標をコピー
    }

    // 繋がりを解除
    vertBuff->Unmap(0, nullptr);

    // 頂点バッファビューの作成
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress(); // GPU仮想アドレス
    vbView.SizeInBytes = sizeVB; // 頂点バッファのサイズ
    vbView.StrideInBytes = sizeof(vertices[0]); // 頂点1つ分のデータサイズ
#pragma endregion

#pragma region ib
    // インデックスデータ全体のサイズ
    UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

    // リソース設定
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeIB;	// インデックス情報が入る分のサイズ
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // インデックスバッファの生成
    result = iDX_->GetDevice()->CreateCommittedResource(
        &heapProp,	// ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &resDesc,	// リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuff));

    // インデックスバッファをマッピング
    uint16_t* indexMap = nullptr;
    result = indexBuff->Map(0, nullptr, (void**)&indexMap);

    // 全インデックスに対して
    for (int i = 0; i < _countof(indices); i++) {
        indexMap[i] = indices[i];	// インデックスをコピー
    }

    // マッピング解除
    indexBuff->Unmap(0, nullptr);

    // インデックスバッファビューの作成
    ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
    ibView.Format = DXGI_FORMAT_R16_UINT;
    ibView.SizeInBytes = sizeIB;
#pragma endregion
}