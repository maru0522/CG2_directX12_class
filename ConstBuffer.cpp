#include "ConstBuffer.h"

template<typename CBType> void ConstBuffer<CBType>::Delete()
{
    // マッピング解除
    constBuff->Unmap(0, nullptr);
}