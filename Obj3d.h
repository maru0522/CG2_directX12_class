#pragma once
#include "WorldTransform.h"
#include "Mesh.h"

class Obj3d
{
public: // ƒƒ“ƒo•Ï”
    WorldTransform worldTransform;

public: // ƒƒ“ƒoŠÖ”
    Obj3d();
    void Update(Matrix4& matView, Matrix4& matProjection);
    void Draw();

private: // ƒƒ“ƒo•Ï”
    Mesh mesh;
};

