#pragma once
#include "Indispensable.h"
#include "IndexBuffer.h"

class IndexBuffer
{

};

#pragma region インデックスデータ
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
#pragma endregion