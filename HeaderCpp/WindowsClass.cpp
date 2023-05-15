#include"WindowsClass.h"


//　ウィンドウプロシャージャ
LRESULT CALLBACK  WindowsClass::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	//　メッセージ固有処理
	switch (msg)
	{
		//　ウィンドウが破棄された
	case WM_DESTROY:
		//　OSに対してアプリが終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	//　メッセージ標準処理
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}




void  WindowsClass::Initialize(const wchar_t* title, const int32_t kClientWidth, const int32_t kClientHeight) {
	//ウィンドウプロシャージャ
	wc.lpfnWndProc = WindowProc;
	// ウィンドウクラス名
	wc.lpszClassName = L"%s",title;
	//インスタンドハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//　カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//ウィンドウクラス登録
	RegisterClass(&wc);
	// クライアント領域サイズ
	
	//  ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,kClientWidth ,kClientHeight };
	// クライアント領域を元に実際のサイズにwrcを変更
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	// ウィンドウ生成
		hwnd = CreateWindow(
			wc.lpszClassName,//　クラス名
			title,                //　タイトルバーの文字
			WS_OVERLAPPEDWINDOW,  //　標準的なウィンドウスタイル
			CW_USEDEFAULT,        //　標準X座標
			CW_USEDEFAULT,        //　標準Y座標
			wrc.right - wrc.left, //　横幅
			wrc.bottom - wrc.top, //　縦幅ti
			nullptr,              //　親ハンドル
			nullptr,              //　メニューハンドル
			wc.hInstance,         //　インスタンスハンドル
			nullptr               //　オプション
		);
	//ウィンドウを表示
	ShowWindow(hwnd, SW_SHOW);
}

