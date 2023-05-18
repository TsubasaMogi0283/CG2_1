#pragma once

#include "Function.h"
 

class DirectXInitialization {
public:
	void DirectXInitialize(int32_t windowsizeWidth,int32_t windowsizeHeight,HWND hwnd);
	
	void DirectXRelease();

private:
	
	ID3D12Resource* vertexResource_ = nullptr;

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

	IDXGISwapChain4* swapChain_ = nullptr;

	ID3D12GraphicsCommandList* commandList_ = nullptr;

	ID3D12CommandAllocator* commandAllocator_ = nullptr;

	ID3D12CommandQueue* commandQueue_ = nullptr;

	ID3D12Device* device_ = nullptr;

	ID3D12InfoQueue* infoQueue_ = nullptr;

	IDXGIAdapter4* useAdapter_ = nullptr;

	IDXGIFactory7* dxgiFactory_ = nullptr;
};
