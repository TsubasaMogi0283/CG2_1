//
#include "WindowsInitialization.h"

WindowsInitialization::WindowsInitialization(const wchar_t* title, const int32_t kClientWidth, const int32_t kClientHeight) {
	this->title_= title;
	this->kClientWidth_ = kClientWidth;
	this->kClientHeight_ = kClientHeight;


}

LRESULT CALLBACK  WindowsInitialization::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// 
//void WindowsInitialization::WindowRegister() {
//
//	
//	
//	//ウィンドウプロシージャ
//	wc_.lpfnWndProc = WindowProc;
//	//ウィンドウクラス名
//	wc_.lpszClassName = L"CG2WindowClass";
//	//インスタンスハンドル
//	wc_.hInstance = GetModuleHandle(nullptr);
//	//カーソル
//	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
//
//	//ウィンドウクラスを登録する
//	RegisterClass(&wc_);
//
//}
//
//void WindowsInitialization::WindowSizeDeside() {
//	//int 32_tを使うためにcstdintをインクルード
//	//クライアント領域のサイズ
//	//クライアント領域・・・ゲーム画面が映る領域のこと
//	
//
//
//	//ウィンドウサイズを表す構造体にクライアント領域を入れる
//	RECT wrc_ = { 0,0,kClientWidth_ ,kClientHeight_ };
//
//	//クライアント領域を元に実際のサイズにwrcを変更してもらう
//	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);
//}
//
//void WindowsInitialization::WindowProdece() {
//	//ウィンドウの生成
//	HWND hwnd_ = CreateWindow(wc_.lpszClassName,L"CG2",					
//		WS_OVERLAPPEDWINDOW,	
//		CW_USEDEFAULT,			
//		CW_USEDEFAULT,			
//		wrc_.right - wrc_.left,	
//		wrc_.bottom - wrc_.top,	
//		nullptr,				
//		nullptr,				
//		wc_.hInstance,			
//		nullptr);				
//
//
//	//利用するクラス名
//	//タイトルバーの文字
//	//よく見るウィンドウスタイル
//	//表示X座標
//	//表示Y座標
//	//ウィンドウ横軸
//	//ウィンドウ縦軸
//	//親ウィンドウハンドル
//	//メニューハンドル
//	//インスタンスハンドル
//	//オプション
//
//}
//
//void WindowsInitialization::WindowInitialize() {
//
//	//WindowRegister();
//
//	//ウィンドウプロシージャ
//	wc_.lpfnWndProc = WindowProc;
//	//ウィンドウクラス名
//	wc_.lpszClassName = L"CG2WindowClass";
//	//インスタンスハンドル
//	wc_.hInstance = GetModuleHandle(nullptr);
//	//カーソル
//	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
//
//	//ウィンドウクラスを登録する
//	RegisterClass(&wc_);
//
//	//出力ウィンドウへの文字出力
//	OutputDebugStringA("Hello,DirectX!\n");
//
//	
//	//WindowSizeDeside();
//
//	RECT wrc_ = { 0,0,kClientWidth_ ,kClientHeight_ };
//
//	//クライアント領域を元に実際のサイズにwrcを変更してもらう
//	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);
//
//	//WindowProdece();
//
//	//ウィンドウの生成
//	HWND hwnd_ = CreateWindow(wc_.lpszClassName,L"CG2",					
//		WS_OVERLAPPEDWINDOW,	
//		CW_USEDEFAULT,			
//		CW_USEDEFAULT,			
//		wrc_.right - wrc_.left,	
//		wrc_.bottom - wrc_.top,	
//		nullptr,				
//		nullptr,				
//		wc_.hInstance,			
//		nullptr);	
//
//
//	////エラー放置ダメ、ゼッタイ
//	////DebugLayer
//	//デバッグレイヤー・・・警告やエラーの出す機構のこと。色々な情報を収集する。
//	//						今までは違法な状態で動いていた。
//	//CreateWindowの直後で行う
//	// 
//	//DirectX12
//	//DXGI
//	//Debug Layer
//	//Graphics Driver
//	//GPU
//
////#ifdef _DEBUG
////	ID3D12Debug1* debugController = nullptr;
////	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
////		//デバッグレイヤーを有効化する
////		debugController->EnableDebugLayer();
////		//さらにGPU側でもチェックを行うようにする
////		debugController->SetEnableGPUBasedValidation(TRUE);
////
////	}
////
////
////#endif
//	
//
//	//ウィンドウを表示する
//	ShowWindow(hwnd_, SW_SHOW);
//
//	
//}




void  WindowsInitialization::WindowInitialize(const wchar_t* title, const int32_t kClientWidth, const int32_t kClientHeight) {
	//ウィンドウプロシャージャ
	wc_.lpfnWndProc = WindowProc;
	// ウィンドウクラス名
	wc_.lpszClassName = L"%s",title_;
	//インスタンドハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//　カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//ウィンドウクラス登録
	RegisterClass(&wc_);
	// クライアント領域サイズ
	
	//  ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc_ = { 0,0,kClientWidth_ ,kClientHeight_ };
	// クライアント領域を元に実際のサイズにwrcを変更
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);
	// ウィンドウ生成
		hwnd_ = CreateWindow(
			wc_.lpszClassName,//　クラス名
			title_,                //　タイトルバーの文字
			WS_OVERLAPPEDWINDOW,  //　標準的なウィンドウスタイル
			CW_USEDEFAULT,        //　標準X座標
			CW_USEDEFAULT,        //　標準Y座標
			wrc_.right - wrc_.left, //　横幅
			wrc_.bottom - wrc_.top, //　縦幅ti
			nullptr,              //　親ハンドル
			nullptr,              //　メニューハンドル
			wc_.hInstance,         //　インスタンスハンドル
			nullptr               //　オプション
		);
	//ウィンドウを表示
	ShowWindow(hwnd_, SW_SHOW);
}

