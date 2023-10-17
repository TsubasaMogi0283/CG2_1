#include "DirectXSetup.h"

static DirectXSetup* instance_;

DirectXSetup::DirectXSetup() {

}

//インスタンス
DirectXSetup* DirectXSetup::GetInstance() {
	//これだと無限に生成されるので
	//ない時に生成する
	if (instance_ == nullptr) {
		instance_ = new DirectXSetup();

	}
	
	return instance_;
}

//デリート代わりの関数
void DirectXSetup::DeleteInstance() {
	delete instance_;
}



//DescriptorHeapの作成関数
ID3D12DescriptorHeap* DirectXSetup::GenarateDescriptorHeap(
		ID3D12Device* device,
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible) {

	ID3D12DescriptorHeap* descriptorHeap= nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));
	return descriptorHeap;

}

ID3D12Resource* DirectXSetup::CreateDepthStencilTextureResource(ID3D12Device* device,int32_t width, int32_t height) {
	D3D12_RESOURCE_DESC resourceDesc{};
	//Textureの幅
	resourceDesc.Width = width;
	//Textureの高さ
	resourceDesc.Height = height;
	//mipmapの数
	resourceDesc.MipLevels = 1;
	//奥行 or 配列Textureの配列数
	resourceDesc.DepthOrArraySize = 1;
	//DepthStencilとして利用可能なフォーマット
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//サンプリングカウント。1固定
	resourceDesc.SampleDesc.Count = 1;
	//2次元
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//DepthStencilとして使う通知
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	//VRAM上に作る
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	//1.0f(最大値)でクリア
	depthClearValue.DepthStencil.Depth = 1.0f;
	//フォーマット。Resourceと合わせる
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//Resourceの作成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,					//Heapの設定 
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定。特になし。
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値を書き込む状態にしておく
		&depthClearValue,					//Clear最適値
		IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));


	return resource;

}




#pragma region Initializeの所で使う関数

void DirectXSetup::GenerateDXGIFactory() {
	//DXGIファクトリーの生成
#ifdef _DEBUG
	//ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		//デバッグレイヤーを有効化する
		debugController_->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController_->SetEnableGPUBasedValidation(TRUE);

	}


#endif 
	
	//関数が成功したかSUCCEEDEDでマクロで判定できる
	hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	//初期でエラーが発生した場合どうにもできないのでassert
	assert(SUCCEEDED(hr_));

}

void DirectXSetup::SelectAdapter() {
	//仕様するアダプタ用の変数、最初にnullptrを入れておく
	//良い順でアダプタを頼む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) !=
		DXGI_ERROR_NOT_FOUND; ++i) {

		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr_ = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr_));

		//ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力.(wstring)
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		//ソフトウェアアダプタだった場合無視
		useAdapter_ = nullptr;


	}
	//適切なアダプタが見つからなかったので起動できない
	assert(useAdapter_ != nullptr);
}

void DirectXSetup::GenerateD3D12Device() {
	//機能レベルとログ出力用の文字
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};
	
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成出来るか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスが生成
		hr_ = D3D12CreateDevice(useAdapter_, featureLevels[i], IID_PPV_ARGS(&device_));
		//指定した機能レベルでデバイスが生成できたか確認
		if (SUCCEEDED(hr_)) {
			//生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}

	//デバイスの生成が上手くいかなかったので起動できない
	assert(device_ != nullptr);
	Log("Complete create D3D12Device!!!\n");


}

void DirectXSetup::StopErrorWarning() {
		////エラー・警告、即ち停止
	//ID3D12InfoQueue* infoQueue_ = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue_)))) {
		//ヤバいエラー時に止まる
		infoQueue_->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue_->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		////全ての情報を出す
		//以下をコメントアウト
		//大丈夫だった場合元に戻してあげる
		infoQueue_->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		
		//解放
		infoQueue_->Release();

		////エラーと警告の抑制
		//Windowsの不具合だと解消できない
		//その時に停止させないよう特定のエラーや警告を無視するしかない

		//抑制するメッセージのID 		
		D3D12_MESSAGE_ID denyIds[] = {
			//Windows11でのDXGデバッグれーやーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		//抑制する
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};

		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue_->PushStorageFilter(&filter);
		
		

	}

}

void DirectXSetup::GenerateCommand() {
	////GPUに作業させよう
	//コマンドキューを生成する
	hr_ = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
	//コマンドキューの生成が上手くいかなかったので起動できない
	assert(SUCCEEDED(hr_));

	//コマンドアロケータを生成する
	//ID3D12CommandAllocator* commandAllocator_ = nullptr;
	hr_ = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	//コマンドアロケータの生成が上手くいかなかったので起動できない
	assert(SUCCEEDED(hr_));

	//コマンドリストを生成する
	//ID3D12GraphicsCommandList* commandList_ = nullptr;
	hr_ = device_->CreateCommandList(
		0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		commandAllocator_, 
		nullptr, 
		IID_PPV_ARGS(&commandList_));

	//コマンドリストの生成が上手くいかなかったので起動できない
	assert(SUCCEEDED(hr_));


}

void DirectXSetup::GenerateSwapChain() {
	
	//60fpsそのまま映すと大変なので2枚用意して
	//描画(フロントバッファ)と表示(バックバッファ、プライマリバッファ)に分ける。
	//このことをダブルバッファリングという。
	//IDXGISwapChain4* swapChain_ = nullptr;
	//DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	swapChainDesc_.Width = kClientWidth_;							//画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Height = kClientHeight_;						//画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			//色の形式
	swapChainDesc_.SampleDesc.Count = 1;							//マルチサンプルしない
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットとして利用する
	swapChainDesc_.BufferCount = 2;								//ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//モニタにうつしたら中身を破棄


	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr_ = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_,
		hwnd_, 
		&swapChainDesc_, 
		nullptr, 
		nullptr, 
		reinterpret_cast<IDXGISwapChain1**>(&swapChain_));
	assert(SUCCEEDED(hr_));
	

}

void DirectXSetup::MakeDescriptorHeap() {
	//ID3D12DescriptorHeap* rtvDescriptorHeap_ = nullptr;
	
	//復習
	//Resourceに対して作業を行うにはViewが必要
	//Viewは作業方法

	//作った関数をここで使う
	rtvDescriptorHeap_ = GenarateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);

	//ImGuiを使うにはSRV用のDescriptorが必要となる
	//SRV...ShaderResourceView
	srvDescriptorHeap_ = GenarateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);

	depthStencilResource_ = CreateDepthStencilTextureResource(device_, kClientWidth_, kClientHeight_);

	dsvDescriptorHeap_ = GenarateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);



	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	//Format 基本的にはResourceに合わせる
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//2DTexture
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//DSVHeapの先頭にDSVを作る
	device_->CreateDepthStencilView(depthStencilResource_, &dsvDesc, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());

	

#pragma region 
	
	//rtvDescriptorHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	//レンダーターゲットビュー用
	//rtvDescriptorHeapDesc_.NumDescriptors = 2;						//ダブルバッファ用に２つ。多くてもOK
	//
	//hr_ = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc_, IID_PPV_ARGS(&rtvDescriptorHeap_));
	////ディスクリプタヒープが作れなかったので起動できない
	//assert(SUCCEEDED(hr_));

	#pragma endregion
	

}

void DirectXSetup::PullResourcesFromSwapChain() {
	//ID3D12Resource* swapChainResources_[2] = { nullptr };
	hr_ = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	//上手く取得できなければ起動できない
	assert(SUCCEEDED(hr_));
	hr_ = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(hr_));

}

void DirectXSetup::SetRTV() {
	//RTVの設定
	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				//出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;			//2dテクスチャとして書き込む
	//ディスクリプタの先頭を取得する
	rtvStartHandle_ = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();


	//RTVを２つ作るのでディスクリプタを２つ用意
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2] = {};
	//まず1つ目を作る。１つ目は最初の所に作る。作る場所をこちらで指定してあげる必要がある
	rtvHandles_[0] = rtvStartHandle_;
	device_->CreateRenderTargetView(swapChainResources_[0], &rtvDesc_, rtvHandles_[0]);
	//２つ目のディスクリプタハンドルを得る(自力で)
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//２つ目を作る
	device_->CreateRenderTargetView(swapChainResources_[1], &rtvDesc_, rtvHandles_[1]);



	////FenceとEvent
	//Fence・・・CPUとGPUの同期を取るために利用するオブジェクト。
	//			 GPUで値を書き込み、CPUで値を読み取ったりWindowsにメッセージ(Event)を送ったりできる
	//			 理想を実現するためのもの
	//Event・・・Windowsへのメッセージなどのこと
	//初期位置0でフェンスを作る
	//EventはWindowsのものである
	
	hr_ = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr_));

	//FenceのSignalを待つためのイベントを作成する
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);






}





void DirectXSetup::GenarateViewport() {
	////ViewportとScissor
	//ビューポート
	
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport_.Width = float(kClientWidth_);
	viewport_.Height = float(kClientHeight_);
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	

}

void DirectXSetup::GenerateScissor() {
	//シザー矩形 
	
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = kClientWidth_;
	scissorRect_.top = 0;
	scissorRect_.bottom = kClientHeight_;
}


#pragma endregion

void DirectXSetup::Initialize() {

	kClientWidth_ = WinApp::GetInstance()->GetClientWidth();
	kClientHeight_ = WinApp::GetInstance()->GetClientHeight();
	hwnd_ = WinApp::GetInstance()->GetHwnd();

	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	//DXGIFactoryを生成
	GenerateDXGIFactory();
	//最適なアダプターを選択
	SelectAdapter();


	//デバイスを生成
	GenerateD3D12Device();

	////エラー・警告、即ち停止
#ifdef _DEBUG
	StopErrorWarning();

#endif 
	

	////GPUに作業させよう
	//コマンドリストを作ろう
	GenerateCommand();


	//スワップチェーンを生成する
	GenerateSwapChain();



	//Resource...DirectX12が管理しているGPU上のメモリであり、このデータのこと
	//View...Resourceに対してどのような処理を行うのか手順をまとめたもの

	//Descriptor...view(作業方法)の情報を格納している場所
	//DescriptorHeap...Descriptorを束ねたもの


	//流れ
	//1.DescriptorHeapを生成する
	//2.swapChainからResourceを引っ張ってくる
	//3.引っ張ってきたResourceに対してDescriptor上にRTVを作る

	////DescriptorHeap(RTV用)を生成する
	MakeDescriptorHeap();

	//スワップチェーンを引っ張ってくる
	PullResourcesFromSwapChain();

	//RenderTargetViewの設定
	SetRTV();


	//DXCの初期化
	////ShaderCompile
	//ShaderはHLSLによって記述されているが、GPUが解釈できる形ではない
	//一度DXIL(DirectX Intermediate Language)というドライバ用の形式に変換され、
	//ドライバがGPU用のバイナリに変更しやっと実行されるよ。手間だね。
	// 
	// DXC(DirectX Shader Compiler)がHLSLからDXILにするCompilerである
	//
	//CompileShaderManager::GetInstance()->InitializeDXC();

	//PSOの生成
	//MakePSO();

	//ビューポートの生成
	GenarateViewport();

	//シザーを生成
	GenerateScissor();



}




void DirectXSetup::BeginFrame() {


	////コマンドをキックする
	//コマンドを積む・・・CommandListに処理を追加していくこと
	//キックする・・・CommandQueueCommandListを渡してGPUの実行を開始すること
	//画面をクリアするためのコマンドを積み、キックし、メインループを完成させる



	//処理の内容
	//1.BackBufferを決定する
	//2.書き込む作業(画面のクリア)をしたいのでRTVを設定する
	//3.画面のクリアを行う
	//4.CommandListを閉じる
	//5.CommandListの実行(キック)
	//6.画面のスワップ(BackBufferとFrontBufferを入れ替える)
	//7.次のフレーム用にCommandListを再準備




	///////
	////コマンドを積みこんで確定させる
	//LoadCommand()
	//これから書き込むバックバッファのインデックスを取得
	backBufferIndex_ = swapChain_->GetCurrentBackBufferIndex();

	////TransitionBarrierを張るコード
	//現在のResourceStateを設定する必要がある → ResorceがどんなStateなのかを追跡する必要がある
	//追跡する仕組みはStateTrackingという
	//
	//TransitionBarrierの設定
	//今回のバリアはTransition
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにする
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier_.Transition.pResource = swapChainResources_[backBufferIndex_];
	//遷移前(現在)のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);



	//描画先のRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex_], false, nullptr);
	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };	//青っぽい色,RGBA
	commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex_], clearColor, 0, nullptr);

	////コマンドを積む
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap_ };

	//描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex_], false, &dsvHandle);
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);

	commandList_->RSSetViewports(1, &viewport_);
	commandList_->RSSetScissorRects(1, &scissorRect_);


	//これをそれぞれに入れたい
	//commandList_->SetGraphicsRootSignature(rootSignature_);
	//commandList_->SetPipelineState(graphicsPipelineState_);
	
}






void DirectXSetup::EndFrame() {
	////画面表示出来るようにする
	//ここがflameの最後
	//画面に描く処理は「全て終わり」、画面に映すので、状態を遷移
	//今回はRenderTargetからPresentにする
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);

	//コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	hr_ = commandList_->Close();
	assert(SUCCEEDED(hr_));



	//コマンドをキックする
	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commandList_ };
	commandQueue_->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うよう通知する



 	swapChain_->Present(1, 0);

	////GPUにSignalを送る
	//GPUの実行完了が目的
	//1.GPUに実行が完了したタイミングでFEnceに指定した値を書き込んでもらう
	//  GPUに対してSignalを発行する
	//	Signal・・・GPUの指定の場所までたどり着いたら、指定の値を書き込んでもらうお願いのこと
	//2.CPUではFenceに指定した値が書き込まれているかを確認する
	//3.指定した値が書き込まれていない場合は、書き込まれるまで待つ
	//Fenceの値を更新
	fenceValue_++;
	//GPUがここまでたどりついた時に、Fenceの値を代入するようSignalを送る
	commandQueue_->Signal(fence_, fenceValue_);
	

	//Fenceの値が指定したSignal値にたどりついているか確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence_->GetCompletedValue() < fenceValue_) {
		//指定したSignalにたどりついていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		//イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}

	

	hr_ = commandAllocator_->Reset();
	assert(SUCCEEDED(hr_));

	hr_ = commandList_->Reset(commandAllocator_, nullptr);
	assert(SUCCEEDED(hr_));
}

void DirectXSetup::Release() {

	
	

	//////解放処理
	CloseHandle(fenceEvent_);
	fence_->Release();

	rtvDescriptorHeap_->Release();
	srvDescriptorHeap_->Release();

	swapChainResources_[0]->Release();
	swapChainResources_[1]->Release();
	swapChain_->Release();

	commandList_->Release();
	commandAllocator_->Release();
	commandQueue_->Release();

	device_->Release();
	useAdapter_->Release();
	dxgiFactory_->Release();

	//////解放処理
	//vertexResource_->Release();


	

#ifdef _DEBUG
	debugController_->Release();

#endif

	
	/*graphicsPipelineState_->Release();
	signatureBlob_->Release();
	if (errorBlob_) {
		errorBlob_->Release();
	}
	
	rootSignature_->Release();

	vertexShaderBlob_->Release();	
	pixelShaderBlob_->Release();*/




}


void DirectXSetup::CheckRelease() {
	////ReportLiveObjects
	//DirectX12より低レベルのDXGIに問い合わせをする
	//リソースリークチェック
	
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug_)))) {
		debug_->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug_->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug_->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug_->Release();
	}
}

DirectXSetup::~DirectXSetup(){

}

