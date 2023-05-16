#include "HeaderCpp/Function.h"
#include "HeaderCpp/WindowsInitialization.h"
#include "HeaderCpp/DirectXInitialization.h"
//includeなどは全部Function.hに入っているよ！



//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;


	WindowsInitialization* window1= new WindowsInitialization(L"l", kClientWidth,kClientHeight);
	DirectXInitialization* directX1 = new DirectXInitialization;


	window1->WindowInitialize();
	directX1->DirectXInitialize();

	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	

#pragma region DirectX初期化
	//ここから↓↓↓
//
//	///////////////
//	IDXGIFactory7* dxgiFactory = nullptr;
//	//関数が成功したかSUCCEEDEDでマクロで判定できる
//	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
//	//初期でエラーが発生した場合どうにもできないのでassert
//	assert(SUCCEEDED(hr));
//
//	////////////
//
//
//
//	//仕様するアダプタ用の変数、最初にnullptrを入れておく
//	IDXGIAdapter4* useAdapter = nullptr;
//	//良い順でアダプタを頼む
//	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
//		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
//		DXGI_ERROR_NOT_FOUND; ++i) {
//
//		//アダプターの情報を取得する
//		DXGI_ADAPTER_DESC3 adapterDesc{};
//		hr = useAdapter->GetDesc3(&adapterDesc);
//		assert(SUCCEEDED(hr));
//
//		//ソフトウェアアダプタでなければ採用
//		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
//			//採用したアダプタの情報をログに出力.(wstring)
//			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
//			break;
//		}
//		//ソフトウェアアダプタだった場合無視
//		useAdapter = nullptr;
//
//
//	}
//	//適切なアダプタが見つからなかったので起動できない
//	assert(useAdapter != nullptr);
//
//	ID3D12Device* device = nullptr;
//	//機能レベルとログ出力用の文字
//	D3D_FEATURE_LEVEL featureLevels[] = {
//		D3D_FEATURE_LEVEL_12_2,
//		D3D_FEATURE_LEVEL_12_1,
//		D3D_FEATURE_LEVEL_12_0
//	};
//
//	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
//	//高い順に生成出来るか試していく
//	for (size_t i = 0; i < _countof(featureLevels); ++i) {
//		//採用したアダプターでデバイスが生成
//		hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));
//		//指定した機能レベルでデバイスが生成できたか確認
//		if (SUCCEEDED(hr)) {
//			//生成できたのでログ出力を行ってループを抜ける
//			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
//			break;
//		}
//	}
//
//	//デバイスの生成が上手くいかなかったので起動できない
//	assert(device != nullptr);
//	Log("Complete create D3D12Device!!!\n");
//
//
//	////エラー・警告、即ち停止
//#ifdef _DEBUG
//	ID3D12InfoQueue* infoQueue = nullptr;
//	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
//		//ヤバいエラー時に止まる
//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
//		//エラー時に止まる
//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
//		//警告時に止まる
//		////全ての情報を出す
//		//以下をコメントアウト
//		//大丈夫だった場合元に戻してあげる
//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
//		//解放
//		infoQueue->Release();
//
//		////エラーと警告の抑制
//		//Windowsの不具合だと解消できない
//		//その時に停止させないよう特定のエラーや警告を無視するしかない
//
//		//抑制するメッセージのID 		
//		D3D12_MESSAGE_ID denyIds[] = {
//			//Windows11でのDXGデバッグれーやーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
//			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
//		};
//
//		//抑制する
//		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
//		D3D12_INFO_QUEUE_FILTER filter{};
//		filter.DenyList.NumIDs = _countof(denyIds);
//		filter.DenyList.pIDList = denyIds;
//		filter.DenyList.NumSeverities = _countof(severities);
//		filter.DenyList.pSeverityList = severities;
//		//指定したメッセージの表示を抑制する
//		infoQueue->PushStorageFilter(&filter);
//
//
//
//	}
//
//#endif 
//
//
//	//ここまで↑↑↑
#pragma endregion 


	

	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームの処理



		}



	}



	////解放処理
	vertexResource->Release();
	graphicsPipelineState->Release();
	signatureBlob->Release();
	if (errorBlob) {
		errorBlob->Release();
	}
	rootSignature->Release();
	pixelShaderBlob->Release();
	vertexShaderBlob->Release();




	////解放処理
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
//#ifdef _DEBUG
//	debugController->Release();
//
//#endif
	CloseWindow(window1->GetHwnd());




	////ReportLiveObjects
	//DirectX12より低レベルのDXGIに問い合わせをする
	//リソースリークチェック
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}





	delete window1;

	return 0;
}


