#pragma once

 #include <Windows.h>
#include <cstdint>

#include "externals/imGui/imgui.h"
#include "externals/imGui/imgui_impl_dx12.h"
#include "externals/imGui/imgui_impl_win32.h"


//extern...グローバル変数を共有する
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

class WindowsInitialization {
public:

	//コンストラクタ
	WindowsInitialization(const wchar_t* title, const int32_t WindowSizeWidth, const int32_t WindowSizeHeight);

	
	//Window Procedure
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	//ウィンドウクラスを登録
	void RegisterWindowsClass();

	//WINodwを表示
	void DisplayWindow();


	void Initialize();





	void WindowsMSG(MSG& msg);


	void Close();


	//Getter
	int GetClientWidth() {
		return kClientWidth_;
	}
	int GetClientHeight() {
		return kClientHeight_;
	}

	HWND GetHwnd() {
		return hwnd_;
	}

private:

	////ウィンドウクラスを登録する
	const wchar_t* title_;
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	

	HWND hwnd_;

	WNDCLASS wc_{};

	

};

