#pragma once
#include "WorldTransform.h"
#include "Mesh.h"

class Obj3d
{
public: // �����o�ϐ�
    WorldTransform worldTransform;

public: // �����o�֐�
    Obj3d();
    void Update(Matrix4& matView, Matrix4& matProjection);
    void Draw();

private: // �����o�ϐ�
    Mesh mesh;
};

