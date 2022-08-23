#pragma once
#include "Indispensable.h"

// 頂点データ構造体
struct Vertex
{
    XMFLOAT3 pos;       // xyz座標
    XMFLOAT3 normal;    // 法線ベクトル
    XMFLOAT2 uv;        // uv座標
};

class VertexBuffer
{
public: // メンバ関数
    VertexBuffer();

    // privateメンバ変数: vbView を取得
    D3D12_VERTEX_BUFFER_VIEW* GetVbView() { return &vbView; }

private: // メンバ変数
// エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12Resource> vertBuff = nullptr;

    // 頂点バッファビューの作成
    D3D12_VERTEX_BUFFER_VIEW vbView{};
};

#pragma region 立方体
Vertex vertices[24] = {
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
#pragma endregion