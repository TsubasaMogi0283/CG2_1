#pragma once

#include <Windows.h>

#include <cstdint>
#include <string>
#include <format>
#include <cassert>


#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

class WindowsInitialization {
public:

	//Window Procedure(関数)
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void WindowInitialize();

	

private:



};

