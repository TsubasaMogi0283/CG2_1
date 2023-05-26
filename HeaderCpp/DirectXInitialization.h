#pragma once

#include "Function.h"
#include "WindowsInitialization.h"
 

class DirectXInitialization {
public:
	

	void MakeDXGIFactory();
	
	void SelectAdapter();

	void MakeD3D12Device();

	void MakeCommandQueue();
	
	void MakeCommandList();
	
	void MakeSwapChain(int32_t windowsizeWidth, int32_t windowsizeHeight,HWND hwnd_);

	void StopErrorWarning();





	void DXCInitialize();

	void DirectXInitialize(int32_t windowsizeWidth, int32_t windowsizeHeight,HWND hwnd_);

	
	

	

	void DirectXRelease();

private:
	int32_t windowsizeWidth_; 
	int32_t windowsizeHeight_; 
	
	HRESULT hr_;

	HWND hwnd_;

	ID3D12Resource* vertexResource_ = nullptr;
	ID3D12Resource* vertexResource1_ = nullptr;

	ID3D12PipelineState* graphicsPipelineState_ = nullptr;

	ID3DBlob* signatureBlob_ = nullptr;

	ID3DBlob* errorBlob_ = nullptr;

	ID3D12RootSignature* rootSignature_ = nullptr;

	IDxcBlob* pixelShaderBlob_ = nullptr;

	IDxcBlob* vertexShaderBlob_ = nullptr;

	HANDLE fenceEvent_ = nullptr;

	ID3D12Fence* fence_ = nullptr;

	ID3D12DescriptorHeap* rtvDescriptorHeap_ = nullptr;

	ID3D12Resource* swapChainResources_[2] = { nullptr };

	

	

	

	

	ID3D12InfoQueue* infoQueue_ = nullptr;

	//
	IDXGIFactory7* dxgiFactory_ = nullptr;
	//
	IDXGIAdapter4* useAdapter_ = nullptr;
	//
	ID3D12Device* device_ = nullptr;
	//
	ID3D12CommandQueue* commandQueue_ = nullptr;

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

	ID3D12CommandAllocator* commandAllocator_ = nullptr;

	ID3D12GraphicsCommandList* commandList_ = nullptr;

	IDXGISwapChain4* swapChain_ = nullptr;
	


#pragma region DXCの初期化について
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;

	IDxcIncludeHandler* includeHandler_ = nullptr;

#pragma endregion


#pragma region PSOについて



#pragma endregion


};
