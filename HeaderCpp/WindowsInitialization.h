#pragma once

#include "Function.h"
 

class WindowsInitialization {
public:

	//コンストラクタ
	WindowsInitialization();

	

	//ウィンドウクラスを登録
	void RegisterWindowsClass();

	//WINodwを表示
	void DisplayWindow();


	void Initialize(const wchar_t* title, const int32_t WindowSizeWidth, const int32_t WindowSizeHeight);





	void WindowsMSG(MSG& msg);


	void Close();


	////Getter
	int GetClientWidth() {
		return kClientWidth_;
	}
	int GetClientHeight() {
		return kClientHeight_;
	}

	
	HWND GetHwnd() {
		return hwnd_;
	}


	~WindowsInitialization();

private:

	////ウィンドウクラスを登録する
	const wchar_t* title_;
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	

	HWND hwnd_;

	WNDCLASS wc_{};

	

};

