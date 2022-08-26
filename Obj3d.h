#pragma once
#include "WorldTransform.h"
#include "Mesh.h"

class Obj3d
{
public: // メンバ変数
    WorldTransform worldTransform;

public: // メンバ関数
    Obj3d();
    void Update(Matrix4& matView, Matrix4& matProjection);
    void Draw();

private: // メンバ変数
    Mesh mesh;
};

