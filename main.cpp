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

#pragma endregion

#pragma region GraphicsPipelineクラス化

    // PSO生成
    GetInstancePSO()->Initialize("BasicVS.hlsl", "BasicPS.hlsl");

#pragma endregion

    // 値を書き込むと自動的に転送される
    obj1.GetCbBuffMaterial().GetBuffMap()->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);		// RGBAで半透明の赤

    // 3Dオブジェクトの数
    const size_t kObjectCount = 10;
    // 3Dオブジェクトの配列
    Object3d obj3ds[kObjectCount];

    // 配列内の全オブジェクトに対して
    for (int i = 0; i < _countof(obj3ds); i++) {
        // 初期化
        obj3ds[i].Initialize();

        // ここからは親子構造のサンプル
        // 先頭以外なら
        if (i > 0) {
            //1つ前のオブジェクトを親オブジェクトとする
            //object3ds[i].parent = &object3ds[i-1];
            // 親オブジェクトの９割の大きさ
            //object3ds[i].scale = { 0.9f,0.9f,0.9f };
            obj3ds[i].SetScale(1.0f, 1.0f, 1.0f);
            // 親オブジェクトに対してZ 軸周りに30度回転
            obj3ds[i].SetRotate(0.0f, 0.0f, 30.0f * i);
            // 親オブジェクトに対してZ方向-8.0ずらす
            obj3ds[i].SetPos(0.0f, 0.0f, -8.0f);
        }
    }

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

        // カメラアップデート
        devCamera.Update();

        /*if (keyboard->isDown(DIK_W) || keyboard->isDown(DIK_S) || keyboard->isDown(DIK_A) || keyboard->isDown(DIK_D)) {
            if (keyboard->isDown(DIK_W)) { object3ds[0].position.y += 1.0f; }
            else if (keyboard->isDown(DIK_S)) { object3ds[0].position.y -= 1.0f; }
            if (keyboard->isDown(DIK_A)) { object3ds[0].position.x -= 1.0f; }
            else if (keyboard->isDown(DIK_D)) { object3ds[0].position.x += 1.0f; }
        }*/

        for (size_t i = 0; i < _countof(obj3ds); i++)             {
            obj3ds[i].Update(devCamera.GetMatView(), devCamera.GetMatProjection());
        }

        // DirectX毎フレーム処理　ここまで

#pragma endregion

        // パイプラインステートとルートシグネチャの設定コマンド
        GetInstanceIDX()->GetCommandList()->SetPipelineState(GetInstancePSO()->GetPipelineState());
        GetInstanceIDX()->GetCommandList()->SetGraphicsRootSignature(GetInstanceRS()->GetRootSignature());

        // プリミティブ形状の設定コマンド
        GetInstanceIDX()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト


        GetInstanceIDX()->GetCommandList()->SetGraphicsRootConstantBufferView(0, obj1.GetCbBuffMaterial().constBuff->GetGPUVirtualAddress());
        // SRVヒープの設定コマンド
        ID3D12DescriptorHeap* srvHeapHandle = GetInstanceIDX()->GetSrvHeap();
        GetInstanceIDX()->GetCommandList()->SetDescriptorHeaps(1, &srvHeapHandle);

        // SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = GetInstanceIDX()->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
        
        // 2枚目を指し示すようにしたSRVのハンドルをルートパラメータ1番に設定
        srvGpuHandle.ptr += tex.GetIncrementSize();
        GetInstanceIDX()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

        // 全オブジェクトについて処理
        for (int i = 0; i < _countof(obj3ds); i++) {
            obj3ds[i].Draw();
        }
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