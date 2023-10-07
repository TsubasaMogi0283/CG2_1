#pragma once

#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "Common/Windows/WinApp.h"
#include "Math/Vector/Vector4.h"


#include <cassert>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")





//メンバ変数関数いつか整理したい・・・
//ごちゃごちゃしてる
class DirectXSetup {
private:
	
	//コンストラクタ
	DirectXSetup();

	//デストラクタ
	~DirectXSetup();

public:
	//インスタンス
	static DirectXSetup* GetInstance();

	//デリート代わりの関数
	void DeleteInstance();

	//コピーコンストラクタ禁止
	DirectXSetup(const DirectXSetup& directXSetup) = delete;

	//代入演算子を無効にする
	DirectXSetup& operator=(const DirectXSetup& directXSetup) = delete;

	//まとめたのが下の「Initialize」
	//他の所では使わないからprivateにしても良さそう
	//アロー演算子を使ったとき邪魔になるから
private:

	//CompilerShader関数
	IDxcBlob* CompileShader(
		//CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		//Compilerに使用するProfile
		const wchar_t* profile,
		//初期化で生成したものを３つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);


	//関数化したやつ

	//DescriptorHeapの作成関数
	ID3D12DescriptorHeap* GenarateDescriptorHeap(
		ID3D12Device* device,
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible);

	//DepthStencilTexture...奥行の根幹をなすものであり、非常に大量の読み書きを高速に行う必要がある
	//						Textureの中でも特に例外的な扱いが必要となっている
	ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device,int32_t width, int32_t height);

	

#pragma region 初期化について
	//初期化へ

	void GenerateDXGIFactory();

	void SelectAdapter();

	void GenerateD3D12Device();

	void StopErrorWarning();

	void GenerateCommand();

	void GenerateSwapChain();

	void MakeDescriptorHeap();

	void PullResourcesFromSwapChain();

	void SetRTV();

	void InitializeDXC();

	void MakePSO();

	void GenarateViewport();

	void GenerateScissor();


#pragma endregion
	

public:

	void Initialize();

	



#pragma region whileの中身
	//whileの中身
	void BeginFrame();

	//void DrawTriangle(Vector4 top, Vector4 left, Vector4 right,TriangleVertex vertexBuffer);

	void EndFrame();

#pragma endregion
	

#pragma region 解放処理
	//解放
	void Release();

	void CheckRelease();

	

#pragma endregion
	




#pragma region 他のクラスでも使いたいのでGetter
	int32_t GetClientWidth() {
		return kClientWidth_;
	}

	int32_t GetClientHeight() {
		return kClientHeight_;
	}
	 


	HRESULT GetHr_() {
		return hr_;
	}

	ID3D12Device* GetDevice() {
		return device_;
	}
	
	ID3D12GraphicsCommandList* GetCommandList() {
		return commandList_;
	}
	


	ID3D12DescriptorHeap* GetRtvDescriptorHeap() {
		return  rtvDescriptorHeap_;
	}
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() {
		return  srvDescriptorHeap_;
	}
	ID3D12DescriptorHeap* GetDsvDescriptorHeap() {
		return  dsvDescriptorHeap_;
	}


	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() {
		return rtvDesc_;
	}
	
	DXGI_SWAP_CHAIN_DESC1 GetswapChainDesc() {
		return swapChainDesc_;
	}

	
	
#pragma endregion

private:
	static DirectXSetup* instance_;


	int32_t kClientWidth_;
	int32_t kClientHeight_;

	HRESULT hr_;

	HWND hwnd_;



	ID3D12Resource* vertexResource_ = nullptr;

	ID3D12PipelineState* graphicsPipelineState_ = nullptr;

	ID3DBlob* signatureBlob_ = nullptr;

	ID3DBlob* errorBlob_ = nullptr;

	ID3D12RootSignature* rootSignature_ = nullptr;

	IDxcBlob* pixelShaderBlob_ = nullptr;

	IDxcBlob* vertexShaderBlob_ = nullptr;




	HANDLE fenceEvent_ = nullptr;


	

	

	

	

	

	ID3D12InfoQueue* infoQueue_ = nullptr;

	//
	IDXGIFactory7* dxgiFactory_ = nullptr;
	//
	IDXGIAdapter4* useAdapter_ = nullptr;

	//
	ID3D12Device* device_ = nullptr;





	ID3D12GraphicsCommandList* commandList_ = nullptr;
	
	ID3D12CommandQueue* commandQueue_ = nullptr;
	
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	
	ID3D12CommandAllocator* commandAllocator_ = nullptr;





	UINT backBufferIndex_;


	//ID3D12DescriptorHeap* srvDescriptorHeap_ = nullptr;
	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	//DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	IDXGISwapChain4* swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	ID3D12Resource* swapChainResources_[2] = { nullptr };

	ID3D12DescriptorHeap* rtvDescriptorHeap_ = nullptr;
	ID3D12DescriptorHeap* srvDescriptorHeap_ = nullptr;
	ID3D12DescriptorHeap* dsvDescriptorHeap_ = nullptr;

	ID3D12Resource* depthStencilResource_ = nullptr;

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};

	

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2] = {};


	

	
	D3D12_RESOURCE_BARRIER barrier_{};

	ID3D12Fence* fence_ = nullptr;

	uint64_t fenceValue_ = 0;


	ID3D12Debug1* debugController_ = nullptr;



#pragma region DXCの初期化について
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;

	IDxcIncludeHandler* includeHandler_ = nullptr;

#pragma endregion

	//PSO
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};


	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};



	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	Vector4* vertexData_ = nullptr;

	IDXGIDebug1* debug_;

};
