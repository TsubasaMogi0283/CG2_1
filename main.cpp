#include "HeaderCpp/Function.h"
#include "HeaderCpp/WindowsInitialization.h"
#include "HeaderCpp/DirectXInitialization.h"
#include "HeaderCpp/DXGIRelease.h"
//includeなどは全部Function.hに入っているよ！



//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;


	WindowsInitialization* window1= new WindowsInitialization(L"l", kClientWidth,kClientHeight);
	DirectXInitialization* directX = new DirectXInitialization();
	DXGIRelease* releaseCheck = new DXGIRelease();

	window1->WindowInitialize();
	

	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	directX->DirectXInitialize(window1->GetClientWidth(), window1->GetClientHeight(), window1->GetHwnd());
	

	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームの処理



		}

	}



	//解放処理
	window1->WindowReset();
	directX->DirectXRelease();

	CloseWindow(window1->GetHwnd());



	releaseCheck->ReleaseCheck();



	delete window1;
	delete directX;
	delete releaseCheck;

	return 0;
}


