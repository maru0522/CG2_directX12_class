#pragma once
#include "Essentials.h"

class DebugCamera
{
public: // �����o�ϐ�
    
public: // �����o�֐�
    void SetCamEye(const float x, const float y, const float z) { eye = { x, y, z }; }
    void SetCamTarget(const float x, const float y, const float z) { target = { x, y, z }; }
    void SetCamUp(const float x,const float y, const float z) { up = { x, y, z }; }

    XMMATRIX GetMatView() const { return matView; }

    void Initialize();
    void Update();

private: // �����o�ϐ�
    XMMATRIX matView;
    XMFLOAT3 eye;
    XMFLOAT3 target;
    XMFLOAT3 up;

private: // �����o�֐�
};

