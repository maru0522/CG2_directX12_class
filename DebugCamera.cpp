#include "DebugCamera.h"

void DebugCamera::Initialize()
{
    matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
    matProjection = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
}
void DebugCamera::InitializeDef()
{
    SetCamEye(120, 0, -100);
    SetCamTarget(0, 0, 0);
    SetCamUp(0, 1, 0);
    matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

    SetFovAngleY(45.0f);
    SetAspectRatioDef();
    SetNearZ(0.1f);
    SetFarZ(1000.0f);
    matProjection = XMMatrixPerspectiveFovLH(fovAngleY,aspectRatio,nearZ,farZ);
}

void DebugCamera::Update()
{
    matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
    matProjection = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
}