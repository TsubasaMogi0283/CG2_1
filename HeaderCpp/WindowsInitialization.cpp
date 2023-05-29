#include "WindowsInitialization.h"


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

WindowsInitialization::WindowsInitialization(const wchar_t* title, const int32_t kClientWidth, const int32_t kClientHeight) {
	this->title_= title;
	this->kClientWidth_ = kClientWidth;
	this->kClientHeight_ = kClientHeight;

	this->hwnd_ = nullptr;
}




void  WindowsInitialization::WindowInitialize() {
	WNDCLASS wc{};
	//ウィンドウプロシャージャ
	wc.lpfnWndProc = WindowProc;
	// ウィンドウクラス名
	wc.lpszClassName = L"%s",title_;
	//インスタンドハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//　カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//ウィンドウクラス登録
	RegisterClass(&wc);
	// クライアント領域サイズ
	
	//  ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc_ = { 0,0,kClientWidth_ ,kClientHeight_ };
	// クライアント領域を元に実際のサイズにwrcを変更
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);
	// ウィンドウ生成
		hwnd_ = CreateWindow(
			wc.lpszClassName,//　クラス名
			title_,                //　タイトルバーの文字
			WS_OVERLAPPEDWINDOW,  //　標準的なウィンドウスタイル
			CW_USEDEFAULT,        //　標準X座標
			CW_USEDEFAULT,        //　標準Y座標
			wrc_.right - wrc_.left, //　横幅
			wrc_.bottom - wrc_.top, //　縦幅ti
			nullptr,              //　親ハンドル
			nullptr,              //　メニューハンドル
			wc.hInstance,         //　インスタンスハンドル
			nullptr               //　オプション
		);

		
#ifdef _DEBUG
	//ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		//デバッグレイヤーを有効化する
		debugController_->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController_->SetEnableGPUBasedValidation(TRUE);

	}


#endif

	//ウィンドウを表示
	ShowWindow(hwnd_, SW_SHOW);
}



void WindowsInitialization::WindowReset() {
#ifdef _DEBUG
	debugController_->Release();

#endif
}
