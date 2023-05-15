
#include "WindowsInitialization.h"

WindowsInitialization::WindowsInitialization() {

}
 
void WindowsInitialization::WindowRegister() {

	
	
	//ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wc_.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc_);

}

void WindowsInitialization::WindowSizeDeside() {
	//int 32_tを使うためにcstdintをインクルード
	//クライアント領域のサイズ
	//クライアント領域・・・ゲーム画面が映る領域のこと
	


	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc_ = { 0,0,kClientWidth_ ,kClientHeight_ };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);
}

void WindowsInitialization::WindowProdece() {
	//ウィンドウの生成
	HWND hwnd_ = CreateWindow(wc_.lpszClassName,L"CG2",					
		WS_OVERLAPPEDWINDOW,	
		CW_USEDEFAULT,			
		CW_USEDEFAULT,			
		wrc_.right - wrc_.left,	
		wrc_.bottom - wrc_.top,	
		nullptr,				
		nullptr,				
		wc_.hInstance,			
		nullptr);				


	//利用するクラス名
	//タイトルバーの文字
	//よく見るウィンドウスタイル
	//表示X座標
	//表示Y座標
	//ウィンドウ横軸
	//ウィンドウ縦軸
	//親ウィンドウハンドル
	//メニューハンドル
	//インスタンスハンドル
	//オプション

}

void WindowsInitialization::WindowInitialize() {

	WindowRegister();

	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	
	WindowSizeDeside();

	WindowProdece();

	


	////エラー放置ダメ、ゼッタイ
	////DebugLayer
	//デバッグレイヤー・・・警告やエラーの出す機構のこと。色々な情報を収集する。
	//						今までは違法な状態で動いていた。
	//CreateWindowの直後で行う
	// 
	//DirectX12
	//DXGI
	//Debug Layer
	//Graphics Driver
	//GPU

#ifdef _DEBUG
	ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);

	}


#endif


	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);

	
}
