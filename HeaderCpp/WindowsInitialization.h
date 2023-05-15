#pragma once

#include "Function.h"
 

class WindowsInitialization {
public:


	WindowsInitialization();

	void WindowRegister();

	void WindowSizeDeside();

	void WindowProdece();

	void WindowInitialize();

	


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
	WNDCLASS wc_{};

	RECT wrc_{};

	HWND hwnd_{};

	const int32_t kClientWidth_= 1280;
	const int32_t kClientHeight_=720;

};

