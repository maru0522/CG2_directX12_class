#pragma once
#include "Indispensable.h"

class Texture
{
public:
public:
    void ClearImgData(XMFLOAT4* _imgData);
    void CreateTexBuff();
    void CreateSrvHeapDesc();
    void CreateSrv();

private:
    // エイリアステンプレート
    template<class T> using Comptr = Microsoft::WRL::ComPtr<T>;
    const size_t texWidth = 256;
    const size_t texHeight = 256;
    const size_t imgDataCount = texWidth * texHeight;

    Comptr<ID3D12Resource> texBuff = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    Comptr<ID3D12DescriptorHeap> srvHeap = nullptr;

private:
};

