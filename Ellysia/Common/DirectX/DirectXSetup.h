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

#include <wrl.h>
using Microsoft::WRL::ComPtr;




//メンバ変数関数いつか整理したい・・・
//ごちゃごちゃしてる
class DirectXSetup {
private:
	
	//コンストラクタ
	DirectXSetup() = default;

	//デストラクタ
	~DirectXSetup() = default;

public:
	//インスタンス
	static DirectXSetup* GetInstance();

	//コピーコンストラクタ禁止
	DirectXSetup(const DirectXSetup& directXSetup) = delete;

	//代入演算子を無効にする
	DirectXSetup& operator=(const DirectXSetup& directXSetup) = delete;

	//まとめたのが下の「Initialize」
	//他の所では使わないからprivateにしても良さそう
	//アロー演算子を使ったとき邪魔になるから
private:


	//DescriptorHeapの作成関数
	ComPtr<ID3D12DescriptorHeap> GenarateDescriptorHeap(
		ComPtr<ID3D12Device> device,
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible);

	//DepthStencilTexture...奥行の根幹をなすものであり、非常に大量の読み書きを高速に行う必要がある
	//						Textureの中でも特に例外的な扱いが必要となっている
	ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(ComPtr<ID3D12Device> device,int32_t width, int32_t height);

	

#pragma region 初期化について
	//初期化へ

	static void GenerateDXGIFactory();

	static void SelectAdapter();

	static void GenerateD3D12Device();

	static void StopErrorWarning();

	static void GenerateCommand();

	static void GenerateSwapChain();

	static void MakeDescriptorHeap();

	static void PullResourcesFromSwapChain();

	static void SetRTV();

	static void GenarateViewport();

	static void GenerateScissor();


#pragma endregion
	

public:

	static void Initialize();

	



#pragma region whileの中身
	//whileの中身
	void BeginFrame();

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

	ComPtr<ID3D12Device> GetDevice() {
		return m_device_;
	}
	
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() {
		return commandList_;
	}
	


	ComPtr<ID3D12DescriptorHeap> GetRtvDescriptorHeap() {
		return  rtvDescriptorHeap_;
	}
	ComPtr<ID3D12DescriptorHeap> GetSrvDescriptorHeap() {
		return  srvDescriptorHeap_;
	}
	ComPtr<ID3D12DescriptorHeap> GetDsvDescriptorHeap() {
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
	


	int32_t kClientWidth_;
	int32_t kClientHeight_;

	HRESULT hr_;

	HWND hwnd_;



	HANDLE fenceEvent_ = nullptr;


	

	

	

	

	

	ComPtr<ID3D12InfoQueue> infoQueue_ = nullptr;

	//
	ComPtr<IDXGIFactory7> m_dxgiFactory_ = nullptr;
	//
	ComPtr<IDXGIAdapter4> m_useAdapter_ = nullptr;

	//
	ComPtr<ID3D12Device> m_device_ = nullptr;





	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;





	UINT backBufferIndex_;


	//ID3D12DescriptorHeap* srvDescriptorHeap_ = nullptr;
	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	//DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	ComPtr<ID3D12Resource> swapChainResources_[2] = { nullptr };

	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;

	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};

	

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2] = {};


	

	
	D3D12_RESOURCE_BARRIER barrier_{};

	ComPtr<ID3D12Fence> fence_ = nullptr;

	uint64_t fenceValue_ = 0;


	ComPtr<ID3D12Debug1> debugController_ = nullptr;




	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};



	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	Vector4* vertexData_ = nullptr;


};
