#pragma once
#include "Indispensable.h"
#include "Window.h"

class DebugCamera
{
public: // メンバ変数
#pragma region ビュー行列
    XMFLOAT3 eye;
    XMFLOAT3 target;
    XMFLOAT3 up;
#pragma endregion

public: // メンバ関数
#pragma region ビュー行列初期化用
    void SetCamEye(const float x, const float y, const float z) { eye = { x, y, z }; }
    void SetCamTarget(const float x, const float y, const float z) { target = { x, y, z }; }
    void SetCamUp(const float x,const float y, const float z) { up = { x, y, z }; }
#pragma endregion

#pragma region 射影行列初期化用
    void SetFovAngleY(float degree) { fovAngleY = XMConvertToRadians(degree); }
    void SetAspectRatioDef() { aspectRatio = static_cast<float>(GetInstanceWnd()->width / GetInstanceWnd()->height); }
    void SetNearZ(float _nearZ) { nearZ = _nearZ; }
    void SetFarZ(float _farZ) { farZ = _farZ; }
#pragma endregion

    // privateメンバ変数: matView を取得する
    XMMATRIX GetMatView() const { return matView; }
    // privateメンバ変数: matProjection を取得する
    XMMATRIX GetMatProjection() const { return matProjection; }

    void Initialize();
    void InitializeDef();
    void Update();

private: // メンバ変数
    XMMATRIX matView;
    XMMATRIX matProjection;

#pragma region 射影行列
    float fovAngleY;
    float aspectRatio;
    float nearZ;
    float farZ;
#pragma endregion
};

