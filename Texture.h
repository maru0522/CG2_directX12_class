#pragma once
#include "Indispensable.h"
#include "InitDirectX.h"

class Texture
{
public: // 静的メンバ関数
    // privateメンバ変数: loadTexNum を取得
    static size_t GetLoadTexNum() { return loadTexNum; }

private: // 静的メンバ変数
    // 読み込まれたテクスチャの数（= 生成されたSRVの数）
    static size_t loadTexNum;

public: // メンバ関数
    Texture(std::string path);

private: // メンバ変数
    // エイリアステンプレート
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D12Resource> texBuff = nullptr;
};
