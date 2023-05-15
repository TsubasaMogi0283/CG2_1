#pragma once

#include "Function.h"
 

class WindowsInitialization {
public:

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	WindowsInitialization(const wchar_t* title, const int32_t kClientWidth, const int32_t kClientHeight);



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

public:

	////ウィンドウクラスを登録する
	WNDCLASS wc_;


	HWND hwnd_;

	const wchar_t* title_;
	int32_t kClientWidth_;
	int32_t kClientHeight_;

};

