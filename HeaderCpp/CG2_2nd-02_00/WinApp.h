#pragma once
#include<Windows.h>
#include<cstdint>
#include"Log.h"
class WinApp
{
public:

	static LRESULT  WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void Initialize(int32_t kClientWidth, int32_t kClientHeight);
	
	HWND GetHWND() const { return hwnd_; };

private:
	HWND hwnd_=nullptr;


};


