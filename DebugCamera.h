#pragma once
#include "Indispensable.h"
#include "Window.h"

class DebugCamera
{
public: // �����o�ϐ�
#pragma region �r���[�s��
    XMFLOAT3 eye;
    XMFLOAT3 target;
    XMFLOAT3 up;
#pragma endregion

public: // �����o�֐�
#pragma region �r���[�s�񏉊����p
    void SetCamEye(const float x, const float y, const float z) { eye = { x, y, z }; }
    void SetCamTarget(const float x, const float y, const float z) { target = { x, y, z }; }
    void SetCamUp(const float x,const float y, const float z) { up = { x, y, z }; }
#pragma endregion

#pragma region �ˉe�s�񏉊����p
    void SetFovAngleY(float degree) { fovAngleY = XMConvertToRadians(degree); }
    void SetAspectRatioDef() { aspectRatio = static_cast<float>(GetInstanceWnd()->width / GetInstanceWnd()->height); }
    void SetNearZ(float _nearZ) { nearZ = _nearZ; }
    void SetFarZ(float _farZ) { farZ = _farZ; }
#pragma endregion

    // private�����o�ϐ�: matView ���擾����
    XMMATRIX GetMatView() const { return matView; }
    // private�����o�ϐ�: matProjection ���擾����
    XMMATRIX GetMatProjection() const { return matProjection; }

    void Initialize();
    void InitializeDef();
    void Update();

private: // �����o�ϐ�
    XMMATRIX matView;
    XMMATRIX matProjection;

#pragma region �ˉe�s��
    float fovAngleY;
    float aspectRatio;
    float nearZ;
    float farZ;
#pragma endregion
};

