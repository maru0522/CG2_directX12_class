#include "Object3d.h"

void Object3d::Initialize()
{
    model = new Model;

    cbBuffMat.Create();
    cbBuffMaterial.Create();
    model->Initialize();
}

void Object3d::Update(const XMMATRIX& _matView, const XMMATRIX& _matProjection)
{
    XMMATRIX matScale, matRot, matTrans;

    // スケール、回転、平行移動行列の計算
    matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
    matRot = XMMatrixIdentity();
    matRot *= XMMatrixRotationZ(rotation.z);
    matRot *= XMMatrixRotationX(rotation.x);
    matRot *= XMMatrixRotationY(rotation.y);
    matTrans = XMMatrixTranslation(
        position.x, position.y, position.x
    );

    // ワールド行列の合成
    matWorld = XMMatrixIdentity();  // 変形をリセット
    matWorld *= matScale;           // ワールド行列にスケーリングを反映
    matWorld *= matRot;             // ワールド行列に回転を反映
    matWorld *= matTrans;           // ワールド行列に平行移動を反映

    // 親オブジェクトがあれば
    if (parent != nullptr) {
        //親オブジェクトのワールド行列をかける
        matWorld *= parent->matWorld;
    }

    // 定数バッファへデータ転送
    cbBuffMat.GetBuffMap()->mat = matWorld * _matView * _matProjection;
}

void Object3d::Draw()
{
    // 頂点バッファの設定
    GetInstanceIDX()->GetCommandList()->IASetVertexBuffers(0, 1, &model->vbView);
    // インデックスバッファの設定
    GetInstanceIDX()->GetCommandList()->IASetIndexBuffer(&model->ibView);
    // 定数バッファビュー（CBV）の設定コマンド
    GetInstanceIDX()->GetCommandList()->SetGraphicsRootConstantBufferView(2, GetCbBuffMat().constBuff->GetGPUVirtualAddress());

    // 描画コマンドリスト
    GetInstanceIDX()->GetCommandList()->DrawIndexedInstanced(model->numIndecies, 1, 0, 0, 0);
}

Object3d::~Object3d()
{
    delete model;
}
