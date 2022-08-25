#include "WorldTransform.h"
#include "InitDirectX.h"

WorldTransform::WorldTransform()
{
}

void WorldTransform::Update(Matrix4& matView, Matrix4& matProjection)
{
    Matrix4 matScale{}, matRot{}, matTrans{};

    // スケール
    matScale = Scale(scale);

    // 回転
    matRot = Identity();
    matRot *= RotateZ(rotation.z);
    matRot *= RotateX(rotation.x);
    matRot *= RotateY(rotation.y);

    // 平行移動行列
    matTrans = Translate(position);

    // ワールド行列の合成
    matWorld = Identity();          // 変形をリセット
    matWorld *= matScale;           // ワールド行列にスケーリングを反映
    matWorld *= matRot;             // ワールド行列に回転を反映
    matWorld *= matTrans;           // ワールド行列に平行移動を反映

    // 親オブジェクトがあれば
    if (parent != nullptr) {
        //親オブジェクトのワールド行列をかける
        matWorld *= parent->matWorld;
    }

    // 定数バッファへデータ転送
    cbTransMap.GetCBTypeMap()->matrix = matWorld * matView * matProjection;
}

void WorldTransform::Draw()
{
    InitDirectX* iDX_ = GetInstanceIDX();

    // 頂点バッファの設定
    iDX_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
    // インデックスバッファの設定
    iDX_->GetCommandList()->IASetIndexBuffer(&ibView);
    // 定数バッファビュー（CBV）の設定コマンド
    iDX_->GetCommandList()->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());

    // 描画コマンドリスト
    iDX_->GetCommandList()->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}
