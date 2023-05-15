
#include "WindowsInitialization.h"

//Window Procedure(関数)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
	WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}



void WindowsInitialization::WindowInitialize() {
	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	////ウィンドウクラスを登録する
	WNDCLASS wc{};
	//ウィンドウプロシージャ
	wc.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wc.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc);



	//int 32_tを使うためにcstdintをインクルード
	//クライアント領域のサイズ
	//クライアント領域・・・ゲーム画面が映る領域のこと
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;


	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,kClientWidth ,kClientHeight };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	HWND hwnd = CreateWindow(
		wc.lpszClassName,		//利用するクラス名
		L"CG2",					//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//よく見るウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標
		CW_USEDEFAULT,			//表示Y座標
		wrc.right - wrc.left,	//ウィンドウ横軸
		wrc.bottom - wrc.top,	//ウィンドウ縦軸
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wc.hInstance,			//インスタンスハンドル
		nullptr);				//オプション


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
	ShowWindow(hwnd, SW_SHOW);

	MSG msg{};
}
