#include "Obj3d.h"
#include "InitDirectX.h"

Obj3d::Obj3d()
{
}

void Obj3d::Update(Matrix4& matView, Matrix4& matProjection)
{
    worldTransform.UpdateMatrix(matView, matProjection);
}

void Obj3d::Draw()
{
    InitDirectX* iDX_ = GetInstanceIDX();

    // 頂点バッファの設定
    iDX_->GetCommandList()->IASetVertexBuffers(0, 1, mesh.GetVbView());
    // インデックスバッファの設定
    iDX_->GetCommandList()->IASetIndexBuffer(mesh.GetIbView());
    // 定数バッファビュー（CBV）の設定コマンド
    iDX_->GetCommandList()->SetGraphicsRootConstantBufferView(2, worldTransform.GetCbTransMap()->GetBuffer()->GetGPUVirtualAddress());

    // 描画コマンドリスト
    iDX_->GetCommandList()->DrawIndexedInstanced(mesh.GetIndices()->size(), 1, 0, 0, 0);
}
