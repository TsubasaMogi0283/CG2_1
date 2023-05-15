#pragma once

#include "Function.h"
 

class WindowsInitialization {
public:


	

	void WindowRegister();

	void WindowSizeDeside();

	void WindowProdece();

	void WindowInitialize();

	

private:

	////ウィンドウクラスを登録する
	WNDCLASS wc_{};

	RECT wrc_{};

	HWND hwnd_{};
};

