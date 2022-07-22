#include "Indispensable.h"
#include "Window.h"
#include "Input.h"
#include "InitDirectX.h"
#include "DebugCamera.h"
#include "Object3d.h"
#include "Texture.h"
#include "PipelineStateObject.h"
#include "RootSignature.h"
#include<math.h>

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region WindowsAPI初期化

    // ウィンドウの作成
    GetInstanceWnd()->CreateObj("MyEngine");

#pragma endregion

#pragma region DirectX初期化

    HRESULT result = S_FALSE;

    // DirectXの初期化
    GetInstanceIDX()->Initialize();

#pragma endregion

#pragma region DirectInput初期化

    // キーボード入力の初期化
    using namespace Input;
    GetInstanceKeys()->Initialize();
#pragma endregion

#pragma region 描画初期化処理
#pragma region Modelクラス化

    Object3d obj1;
    obj1.Initialize();
    obj1.SetPos(0, 0, 0);

#pragma endregion

#pragma region GraphicsPipelineクラス化

    // PSO生成
    GetInstancePSO()->Initialize("BasicVS.hlsl", "BasicPS.hlsl");

#pragma endregion

    float R = 1.0f;
    float G = 0.0f;
    float B = 1.0f;
    float A = 1.0f;

    bool isChengeColor = true;

    // 値を書き込むと自動的に転送される
    obj1.GetCbBuffMaterial().GetBuffMap()->color = XMFLOAT4(R, G, B, A);		// RGBA

    // ビュー変換行列（グローバル変数）
    DebugCamera devCamera;
    devCamera.InitializeDef();


#pragma region テクスチャマッピング

    Texture tex;
    tex.LoadWICTex("Resources/mario.jpg");

#pragma endregion
#pragma endregion


    // ゲームループ
    while (true) {
#pragma region ウィンドウメッセージ処理

        if (!GetInstanceWnd()->IsKeep()) {
            break;
        }

#pragma endregion

#pragma region DirextX毎フレーム処理

        // DirectX毎フレーム処理　ここから
        GetInstanceIDX()->PreDraw();

#pragma region キーボード情報の取得

        // キーボード更新
        GetInstanceKeys()->Update();

#pragma endregion

        /*ここから更新処理*/
        if (isChengeColor) {
            R -= 0.01f;
            G += 0.01f;
            B -= 0.01f;
        }
        else {
            R += 0.01f;
            G -= 0.01f;
            B += 0.01f;
        }

        if (R >= 0.9) {
            isChengeColor = true;
        }
        else if (R <= 0.1) {
            isChengeColor = false;
        }

        if (GetInstanceKeys()->isDown(DIK_Q) || GetInstanceKeys()->isDown(DIK_E)) {
            if (GetInstanceKeys()->isDown(DIK_Q)) {
                if (A < 1.0f) {
                    A += 0.01f;
                }
            }
            if (GetInstanceKeys()->isDown(DIK_E)) {
                if (A > 0.0f) {
                    A -= 0.01f;
                }
            }
        }


        // 値を書き込むと自動的に転送される
        obj1.GetCbBuffMaterial().GetBuffMap()->color = XMFLOAT4(R, G, B, A);

        if (GetInstanceKeys()->isDown(DIK_W) || GetInstanceKeys()->isDown(DIK_S) ||
            GetInstanceKeys()->isDown(DIK_A) || GetInstanceKeys()->isDown(DIK_D)) {
            if (GetInstanceKeys()->isDown(DIK_W)) { obj1.position.y += 0.5f; }
            else if (GetInstanceKeys()->isDown(DIK_S)) { obj1.position.y -= 0.5f; }
            if (GetInstanceKeys()->isDown(DIK_A)) { obj1.position.x -= 0.5f; }
            else if (GetInstanceKeys()->isDown(DIK_D)) { obj1.position.x += 0.5f; }
        }

        if (GetInstanceKeys()->isDown(DIK_LEFTARROW) || GetInstanceKeys()->isDown(DIK_RIGHTARROW) ||
            GetInstanceKeys()->isDown(DIK_UPARROW) || GetInstanceKeys()->isDown(DIK_DOWNARROW)) {
            if (GetInstanceKeys()->isDown(DIK_LEFTARROW)) { obj1.rotation.y += 0.05f; }
            if (GetInstanceKeys()->isDown(DIK_RIGHTARROW)) { obj1.rotation.y -= 0.05f; }
            if (GetInstanceKeys()->isDown(DIK_UPARROW)) { obj1.rotation.x += 0.05f; }
            if (GetInstanceKeys()->isDown(DIK_DOWNARROW)) { obj1.rotation.x -= 0.05f; }
        }

        if (GetInstanceKeys()->isDown(DIK_LSHIFT)) {
            if (GetInstanceKeys()->isDown(DIK_NUMPAD8) || GetInstanceKeys()->isDown(DIK_NUMPAD2) ||
                GetInstanceKeys()->isDown(DIK_NUMPAD4) || GetInstanceKeys()->isDown(DIK_NUMPAD6)) {
                if (GetInstanceKeys()->isDown(DIK_NUMPAD8)) { devCamera.target.y += 0.1f; }
                if (GetInstanceKeys()->isDown(DIK_NUMPAD2)) { devCamera.target.y -= 0.1f; }
                if (GetInstanceKeys()->isDown(DIK_NUMPAD4)) { devCamera.target.x -= 0.1f; }
                if (GetInstanceKeys()->isDown(DIK_NUMPAD6)) { devCamera.target.x += 0.1f; }
            }
        }
        else if (GetInstanceKeys()->isDown(DIK_NUMPAD8) || GetInstanceKeys()->isDown(DIK_NUMPAD2) ||
                 GetInstanceKeys()->isDown(DIK_NUMPAD4) || GetInstanceKeys()->isDown(DIK_NUMPAD6)) {
            if (GetInstanceKeys()->isDown(DIK_NUMPAD8)) { devCamera.eye.y += 0.1f; }
            if (GetInstanceKeys()->isDown(DIK_NUMPAD2)) { devCamera.eye.y -= 0.1f; }
            if (GetInstanceKeys()->isDown(DIK_NUMPAD4)) { devCamera.eye.x += 0.1f; }
            if (GetInstanceKeys()->isDown(DIK_NUMPAD6)) { devCamera.eye.x -= 0.1f; }
        }


        // カメラアップデート
        devCamera.Update();
        obj1.Update(devCamera.GetMatView(), devCamera.GetMatProjection());

        /*更新処理ここまで*/
        // DirectX毎フレーム処理　ここまで

#pragma endregion

        // パイプラインステートとルートシグネチャの設定コマンド
        GetInstanceIDX()->GetCommandList()->SetPipelineState(GetInstancePSO()->GetPipelineState());
        GetInstanceIDX()->GetCommandList()->SetGraphicsRootSignature(GetInstanceRS()->GetRootSignature());

        // プリミティブ形状の設定コマンド
        GetInstanceIDX()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト


        GetInstanceIDX()->GetCommandList()->SetGraphicsRootConstantBufferView(0, obj1.GetCbBuffMaterial().constBuff->GetGPUVirtualAddress());
        //GetInstanceIDX()->GetCommandList()->SetGraphicsRootConstantBufferView(0, obj2.GetCbBuffMaterial().constBuff->GetGPUVirtualAddress());

        // SRVヒープの設定コマンド
        ID3D12DescriptorHeap* srvHeapHandle = GetInstanceIDX()->GetSrvHeap();
        GetInstanceIDX()->GetCommandList()->SetDescriptorHeaps(1, &srvHeapHandle);

        // SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = GetInstanceIDX()->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();

        // 2枚目を指し示すようにしたSRVのハンドルをルートパラメータ1番に設定
        srvGpuHandle.ptr += tex.GetIncrementSize();
        GetInstanceIDX()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

        // objDraw
        obj1.Draw();

        // ４．描画コマンドここまで

#pragma endregion
#pragma region 画面入れ替え

        GetInstanceIDX()->PostDraw();

#pragma endregion

        GetInstanceIDX()->ReadyAgainCmd();
    }
    GetInstanceWnd()->DeleteObj();
    return 0;
}