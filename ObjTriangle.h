#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include<d3d12.h>
#include<dxgi1_6.h>
#include<d3dcompiler.h>
#include<cassert>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<vector>
#include<string>
#include<DirectXTex.h>

#define DIRECTINPUT_VERSION 0x0800
class ObjTriangle
{
public:
	ObjTriangle();
	~ObjTriangle();

	void Initialize();
	void Update();

private:
	// 頂点数
	static const int kVertNum = 4;
	// インデックス数
	static const int kIdxNum = 6;

#pragma region DirectX初期化
	//DirectX初期化
	HRESULT sResult;
	ID3D12Device* sDevice;
	/*IDXGIFactory7* dxgiFactory;
	IDXGISwapChain4* swapChain;
	ID3D12CommandAllocator* cmdAllocator;
	ID3D12GraphicsCommandList* commandList;
	ID3D12CommandQueue* commandQueue;
	ID3D12DescriptorHeap* rtvHeap;*/

#pragma region アダプタの列挙
	// オブジェクトを増やすごとにアダプタを列挙する処理は非効率だと考え、クラスでのアダプタの列挙はしない。
	/*std::vector<IDXGIAdapter4*> adapters;*/
	// ここに特定の名前を持つアダプターオブジェクトが入る
	/*IDXGIAdapter4* tmpAdapter;*/
#pragma endregion

#pragma region デバイスの生成
	// オブジェクトを増やすごとにデバイスを生成する処理は非効率だと考え、クラスでのデバイスの生成はしない。
	// 対応レベルの配列
	/*D3D_FEATURE_LEVEL levels[4];*/

	/*D3D_FEATURE_LEVEL featureLevel;*/
#pragma endregion

#pragma region コマンドキューの生成
	// オブジェクトを増やすごとに実行命令が増えていく処理は非効率だと考え、クラスでのキューの生成はしない。
	//コマンドキューの設定
	/*D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};*/
#pragma endregion

#pragma region スワップチェーンの生成
	// オブジェクトを増やすごとに2つのバッファー分のメモリを確保するのは非効率だと考え、クラスでのスワップチェーンの生成はしない。
	// スワップチェーンの設定
	/*DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};*/
#pragma endregion

#pragma region レンダービューターゲット
	// オブジェクトを増やすごとにバッファの書き換え処理が増えていくのは非効率だと考え、クラスでのレンダーターゲットビューの生成はしない。
	// デスクリプタヒープの設定
	/*D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};*/

	// SRVの最大個数
	/*const size_t kMaxSRVCount;*/

	// デスクリプタヒープの設定
	/*D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};*/

	// 設定を元にSRV用デスクリプタヒープを作成
	/*ID3D12DescriptorHeap* srvHeap;*/

	// SRVヒープの先頭ハンドルを取得
	/*D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;*/

	// バックバッファ
	/*std::vector<ID3D12Resource*> backBuffers;*/
#pragma endregion

#pragma region フェンス
	// オブジェクトを増やすごとにGPUの処理が完了したかを監視する処理が増えていくのは非効率だと考え、クラスでのフェンス生成はしない。
	// フェンスの生成
	/*ID3D12Fence* fence;*/
	/*UINT64 fenceVal;*/
#pragma endregion
#pragma endregion

#pragma region 描画初期化処理変数
	// 頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;	// xyz座標
		XMFLOAT2 uv;	// uv座標
	};

	// 頂点データ
	Vertex vertices[kVertNum];

	// インデックスデータ
	unsigned short indices[kIdxNum];

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB;

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	// 頂点バッファの生成
	ID3D12Resource* vertBuff;

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

#pragma region 頂点シェーダー変数
	ID3DBlob* vsBlob; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob; // エラーオブジェクト
#pragma endregion

#pragma region 頂点レイアウト
// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];
#pragma endregion

#pragma region グラフィックスパイプライン設定
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
#pragma region ブレンドステート
	// --コンストラクタでエラーが出るため仕方なく初期化。
	// --142行目の pipelineDescを初期化子で初期化しているためおそらく0が入るはず。
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
#pragma endregion
	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	// ルートパラメーターの設定
	D3D12_ROOT_PARAMETER rootParams[2] = {};

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//ルートシグネチャ
	ID3D12RootSignature* rootSignature;

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};

	// ルートシグネチャのシリアライズ
	ID3D10Blob* rootSigBlob;
#pragma endregion

#pragma region パイプラインステートの生成
	// パイプランステートの生成
	ID3D12PipelineState* pipelineState;
#pragma endregion

	struct ConstBufferDataMaterial {
		XMFLOAT4 color;		// 色（RGBA）
	};

	// 定数バッファ生成用の設定
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};

	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};

	// 定数バッファの生成
	ID3D12Resource* constBuffMaterial;

	// 定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial;

#pragma region テクスチャマッピング
	// 横方向ピクセル数
	const size_t textureWidth = 256;
	// 縦方向ピクセル数
	const size_t textureHeight = 256;
	// 配列の要素数
	const size_t imageDataCount = textureWidth * textureHeight;

	//画像イメージデータ配列
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	ScratchImage mipChain{};

	// 画像イメージデータ配列
	XMFLOAT4* imageData;

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};

	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};

	// テクスチャバッファの生成
	ID3D12Resource* texBuff;

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
#pragma endregion

	// インデックスデータ全体のサイズ
	UINT sizeIB;

	// インデックスバッファの生成
	ID3D12Resource* indexBuff;

	// インデックスバッファをマッピング
	uint16_t* indexMap;

	// インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
#pragma endregion

#pragma region グラフィックスコマンド
	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};
#pragma endregion
};
