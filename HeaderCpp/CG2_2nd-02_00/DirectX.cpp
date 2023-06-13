#include"DirectX.h"



void DirectX::Initialize(WinApp* winApp,int32_t kClientWidth, int32_t kClientHeight)
{
	winApp_ = winApp;
	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;
#ifdef _DEBUG
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバッグレイヤ―を有効化
		debugController->EnableDebugLayer();
		//GPU側のチェック
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif
	
	MakeDXGIFactory();

	MakeD3D12Device();
#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//やばいエラー
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		//リリース
		infoQueue->Release();
		//エラーの抑制
		D3D12_MESSAGE_ID denyIds[] = {
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		infoQueue->PushStorageFilter(&filter);
	}
#endif
	MakeCommandQueue();

	MakeCommandAllocator();

	MakeCommandList();

	MakeSwapChain();
	
	MakeDescriptorHeap();

	MakeFence();

	MakeDXC();

	MakeRootSignature();

	MakeInputLayOut();

	MakeBlendState();

	MakeRasterizarState();

	MakeShaderCompile();

	MakePipelineStateObject();

	MakeViewport();

	MakeScissor();
}

void DirectX::PreView()
{
	//コマンドを積む
	//バックバッファのインデックス取得
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを貼る対象のリソース
	barrier.Transition.pResource = swapChainResources[backBufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

	//描画先のRTVの設定
	commandList->OMSetRenderTargets(1,&rtvHandles[backBufferIndex],false,nullptr);
	//指定した色で画面全体をクリア
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };//いつもの青っぽいやつ
	commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);
	
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);
	commandList->SetGraphicsRootSignature(rootSignature);
	commandList->SetPipelineState(graphicsPipelineState);
}

void DirectX::PostView()
{
	//RenderTargetからPresentにする
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

	hr = commandList->Close();
	assert(SUCCEEDED(hr));
	//コマンドをキック
	ID3D12CommandList* commandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandLists);
	swapChain->Present(1, 0);
	//Fenceの更新
	fenceValue++;
	commandQueue->Signal(fence, fenceValue);
	if (fence->GetCompletedValue() < fenceValue) {
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}
	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList->Reset(commandAllocator, nullptr);
	assert(SUCCEEDED(hr));

}

void DirectX::Release()
{
	CloseHandle(fenceEvent);
	fence->Release();
	rtvDescriptorHeap->Release();
	swapChainResources[0]->Release();
	swapChainResources[1]->Release();
	swapChain->Release();
	commandList->Release();
	commandAllocator->Release();
	commandQueue->Release();
	device->Release();
	useAdapter->Release();
	dxgiFactory->Release();
#ifdef _DEBUG
	debugController->Release();
#endif
	CloseWindow(winApp_->GetHWND());
	graphicsPipelineState->Release();
	signatureBlob->Release();
	if (errorBlob) {
		errorBlob->Release();
	}
	rootSignature->Release();
	pixelShaderBlob->Release();
	vertexShaderBlob->Release();
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
}


//プライベート関数
void DirectX::MakeDXGIFactory()
{
	//DXGIファクトリーを作成
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(hr));
	//アダプターを作成
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; i++) {
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;
	}
	//アダプターが見つからないので起動できない
	assert(useAdapter != nullptr);
}

void DirectX::MakeD3D12Device()
{
	//機能レベル
	D3D_FEATURE_LEVEL featureLevels[]{
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試す
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスを生成
		hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));
		if (SUCCEEDED(hr)) {
			//生成出来たらログを出力してループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//デバイスの生成ができないので起動できない
	assert(device != nullptr);
	//初期化完了のログを出す
	Log("Complete create D3D12Device!!!\n");
}

void DirectX::MakeCommandQueue()
{
	//コマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	//コマンドキューの生成ができないので起動できない
	assert(SUCCEEDED(hr));

}

void DirectX::MakeCommandAllocator()
{
	//コマンドアロケータを作成
	hr = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		IID_PPV_ARGS(&commandAllocator));
	//コマンドアロケータの生成ができないので起動できない
	assert(SUCCEEDED(hr));
}

void DirectX::MakeCommandList()
{
	//コマンドリストを作成
	hr = device->CreateCommandList(
		0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	//コマンドリストの生成ができないので起動できない
	assert(SUCCEEDED(hr));
}

void DirectX::MakeSwapChain()
{
	//スワップチェーンを作成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = kClientWidth_;
	swapChainDesc.Height = kClientHeight_;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成
	hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue, winApp_->GetHWND(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain));
	//スワップチェーンの生成ができないので起動できない
	assert(SUCCEEDED(hr));
}

void DirectX::MakeDescriptorHeap()
{
	//ディスクリプタヒープの作成
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeapDesc.NumDescriptors = 2;
	hr = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
	//ディスクリプタヒープの生成ができないので起動できない
	assert(SUCCEEDED(hr));
	
	//SwapChainからResourceを持ってくる
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	//リソースの取得ができないので起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	//リソースの取得ができないので起動できない
	assert(SUCCEEDED(hr));
	
	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//ディスクリプタの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//一つ目
	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResources[0], &rtvDesc, rtvHandles[0]);
	//二つ目
	rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device->CreateRenderTargetView(swapChainResources[1], &rtvDesc, rtvHandles[1]);
}

void DirectX::MakeFence()
{
	//Fenceを作る
	hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));
	//FenceのSignalを待つイベント
	assert(fenceEvent != nullptr);
}

void DirectX::MakeDXC()
{
	//dxCompiler初期化
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));

	
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));
}

IDxcBlob* DirectX::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
	//ログにメッセージ
	Log(ConvertString(std::format(L"Begin CompileShader, path:{},profile:{}\n",filePath,profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult)
	);
	//dxcが起動できない
	assert(SUCCEEDED(hr));

	
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}
	
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	shaderSource->Release();
	shaderResult->Release();
	return shaderBlob;
}

void DirectX::MakeRootSignature()
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

void DirectX::MakeInputLayOut()
{
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);
}

void DirectX::MakeBlendState()
{
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

void DirectX::MakeRasterizarState()
{
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}

void DirectX::MakeShaderCompile()
{
	vertexShaderBlob = CompileShader(L"Object3D.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob != nullptr);
	pixelShaderBlob = CompileShader(L"Object3D.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
}

void DirectX::MakePipelineStateObject()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature;
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	//書き込むRTV
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void DirectX::MakeViewport()
{
	viewport.Width = (float)kClientWidth_;
	viewport.Height = (float)kClientHeight_;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void DirectX::MakeScissor()
{
	scissorRect.left = 0;
	scissorRect.right = kClientWidth_;
	scissorRect.top = 0;
	scissorRect.bottom = kClientHeight_;
}


