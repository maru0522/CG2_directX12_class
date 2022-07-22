#include "PipelineStateObject.h"
#include "InitDirectX.h"
#include "RootSignature.h"

namespace {
    // ワイド文字に変換
    std::wstring ConvertString(const std::string& text) {
        if (text.empty()) {
            return std::wstring();
        }

        /*
         * ref: https://docs.microsoft.com/ja-JP/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
         * ref: http://www.t-net.ne.jp/~cyfis/win_api/sdk/MultiByteToWideChar.html
         * ref: http://chokuto.ifdef.jp/urawaza/api/MultiByteToWideChar.html
         */
        int wcharNum =
            MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), NULL, 0);
        if (wcharNum == 0) {
            return std::wstring();
        }
        std::wstring result(wcharNum, 0);
        MultiByteToWideChar(
            CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), &result[0], wcharNum);
        return result;
    }
}

void PipelineStateObject::LoadVertShader(std::string _vertShader)
{
    HRESULT result = S_FALSE;
    Comptr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

    std::wstring shaderFile = ConvertString(_vertShader);

    // 頂点シェーダの読み込みとコンパイル
    result = D3DCompileFromFile(
        shaderFile.c_str(), // シェーダファイル名
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
        "main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
        0,
        &vsBlob, &errorBlob);

    // エラーなら
    if (FAILED(result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
                    errorBlob->GetBufferSize(),
                    error.begin());
        error += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(error.c_str());
        assert(0);
    }
}

void PipelineStateObject::LoadPixelShader(std::string _pixelShader)
{
    HRESULT result = S_FALSE;
    Comptr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

    std::wstring shaderFile = ConvertString(_pixelShader);

    // ピクセルシェーダの読み込みとコンパイル
    result = D3DCompileFromFile(
        shaderFile.c_str(), // シェーダファイル名
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
        "main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
        0,
        &psBlob, &errorBlob);

    // エラーなら
    if (FAILED(result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
                    errorBlob->GetBufferSize(),
                    error.begin());
        error += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(error.c_str());
        assert(0);
    }
}

void PipelineStateObject::Initialize(std::string _vertShader, std::string _pixelShader)
{
    HRESULT result = S_FALSE;

    LoadVertShader(_vertShader);
    LoadPixelShader(_pixelShader);

    // グラフィックスパイプライン設定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

    // シェーダーの設定
    pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

    // サンプルマスクの設定
    pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

    // ラスタライザの設定
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;   // 背面をカリング
    pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;  // ポリゴン内塗りつぶし
    pipelineDesc.RasterizerState.DepthClipEnable = true;            // 深度クリッピングを有効に

    // ブレンドステート
    /*pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;*/
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画

    // 共通設定(アルファ値)
    blenddesc.BlendEnable = true;					// ブレンドを有効にする
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// 加算
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// ソースの値を100%使う
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// デストの値を0%使う

    // 加算合成(RGB値)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;		// 加算
    blenddesc.SrcBlend = D3D12_BLEND_ONE;		// ソースの値を100%使う
    blenddesc.DestBlend = D3D12_BLEND_ONE;		// デストの値を100%使う

    // 減算合成(RGB値)
    blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;		// デストカラソースヲを減算
    blenddesc.SrcBlend = D3D12_BLEND_ONE;					// ソースの値を100%使う
    blenddesc.DestBlend = D3D12_BLEND_ONE;					// デストの値を100%使う

    // 色反転(RGB値)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;					// 加算
    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;		// 1.0f - デストカラーの値
    blenddesc.DestBlend = D3D12_BLEND_ZERO;					// 使わない

    // 半透明合成
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;					// 加算
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				// ソースのアルファ値
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		// 1.0f - ソースのアルファ値

    // 頂点レイアウトの設定
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

    // 図形の形状設定
    pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // その他の設定
    pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
    pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
    pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

    // ルートシグネチャを初期化
    GetInstanceRS()->Initialize();

    // パイプラインにルートシグネチャをセット
    pipelineDesc.pRootSignature = GetInstanceRS()->GetRootSignature();

    // デプスステンシルステートの設定
    pipelineDesc.DepthStencilState.DepthEnable = true;  // 深度テストを行う
    pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;     // 書き込み許可
    pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;          // 小さければ合格
    pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    // パイプランステートの生成
    result = GetInstanceIDX()->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
    assert(SUCCEEDED(result));
}

PipelineStateObject pipelineStateObject;
PipelineStateObject* GetInstancePSO()
{
    return &pipelineStateObject;
}