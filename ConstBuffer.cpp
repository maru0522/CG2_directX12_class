#include "ConstBuffer.h"

template<typename CBType> void ConstBuffer<CBType>::Delete()
{
    // �}�b�s���O����
    constBuff->Unmap(0, nullptr);
}