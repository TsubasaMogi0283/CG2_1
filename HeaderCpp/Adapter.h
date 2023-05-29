#pragma once
#include "Function.h"
#include "WindowsInitialization.h"
#include "DirectXInitialization.h"

class Adapter{
public:
	Adapter();
	~Adapter();

	void Initialize(const wchar_t* title, const int32_t Width, const int32_t Height);

	void WinMSG(MSG &msg);

	void BeginFlame(const int32_t kClientWidth, const int32_t kClientHeight);

	void EndFlame();

	void TriangleDraw(Vector4 top, Vector4 left,Vector4 right);

	void Deleate();

private:


	WindowsInitialization* windowsSetup_ = new WindowsInitialization();
	DirectXInitialization* directXSetup_ = new DirectXInitialization();

	
};


