#pragma once

 #include <Windows.h>
#include <cstdint>

#include "externals/imGui/imgui.h"
#include "externals/imGui/imgui_impl_dx12.h"
#include "externals/imGui/imgui_impl_win32.h"


//extern...グローバル変数を共有する
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

//シングルでやりたい
//finalで継承禁止
class WinApp final{
private:
	//インスタンスを作れないようにする
	//コンストラクタをprivateに
	WinApp();

	//デストラクタも
	~WinApp();

public:

	#pragma region 禁止事項
	//コピーコンストラクタ禁止
	WinApp(const WinApp& winApp) = delete;

	//代入演算子を無効にする
	WinApp& operator=(const WinApp& winApp) = delete;

	#pragma endregion

	//インスタンスにアクセス可能な関数を追加
	//静的メンバ関数にする。クラス名を指定すればアクセスできる
	static WinApp* GetInstance();

	//Window Procedure
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);



private:
	//ここでしか使わない関数はprivateにした方がいい

	//ウィンドウクラスを登録
	void RegisterWindowsClass();

	//WINodwを表示
	void DisplayWindow();


public:

	//初期化
	void Initialize(const wchar_t* title, int32_t clientWidth,int32_t clientHeight);


	void WindowsMSG(MSG& msg);


	void Close();

#pragma region アクセッサ

	//Getter
	int GetClientWidth() {
		return clientWidth_;
	}
	int GetClientHeight() {
		return clientHeight_;
	}

	HWND GetHwnd() {
		return hwnd_;
	}

	HINSTANCE GetHInstance() {
		return wc_.hInstance;
	}

	wchar_t* SetClassName(wchar_t* name) {
		title_ = name;
	}

#pragma endregion

private:

	////ウィンドウクラスを登録する
	const wchar_t* title_=L"Ellysia";
	
	int32_t clientWidth_=0;
	int32_t clientHeight_=0;
	

	HWND hwnd_=0;

	WNDCLASS wc_{};

	

};

