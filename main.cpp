#include "HeaderCpp/CG2_2nd-02_00/WinApp.h"
#include"HeaderCpp/CG2_2nd-02_00/DirectX.h"
#include"HeaderCpp/CG2_2nd-02_00/DrawEngine.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//ウィンドウクラス
	WinApp* winApp = new WinApp();
	int32_t kClientWidth = 1280;
	int32_t kClientHeight = 720;

	winApp->Initialize(kClientWidth, kClientHeight);
	//DirectX
	DirectX* directX = new DirectX();
	directX->Initialize(winApp, kClientWidth, kClientHeight);
	DrawEngine* drawEngine[10];
	for (int i = 0; i < 10; i++) {
		drawEngine[i] = new DrawEngine();
		drawEngine[i]->Initialize(directX);
	}
	MSG msg{};
	Vector4 Left[10] = {
		{-0.1f,0.9f,0.0f,1.0f},
		{-0.1f,0.8f,0.0f,1.0f},
		{-0.1f,0.7f,0.0f,1.0f},
		{-0.1f,0.6f,0.0f,1.0f},
		{-0.1f,0.5f,0.0f,1.0f},
		{-0.1f,0.4f,0.0f,1.0f},
		{-0.1f,0.3f,0.0f,1.0f},
		{-0.1f,0.2f,0.0f,1.0f},
		{-0.1f,0.1f,0.0f,1.0f},
		{-0.1f,0.0f,0.0f,1.0f}
	};
	Vector4 Top[10] = {
		{0.0f,1.0f,0.0f,1.0f},
		{0.0f,0.9f,0.0f,1.0f},
		{0.0f,0.8f,0.0f,1.0f},
		{0.0f,0.7f,0.0f,1.0f},
		{0.0f,0.6f,0.0f,1.0f},
		{0.0f,0.5f,0.0f,1.0f},
		{0.0f,0.4f,0.0f,1.0f},
		{0.0f,0.3f,0.0f,1.0f},
		{0.0f,0.2f,0.0f,1.0f},
		{0.0f,0.1f,0.0f,1.0f}
	};
	Vector4 Right[10] = {
		{0.1f,0.9f,0.0f,1.0f},
		{0.1f,0.8f,0.0f,1.0f},
		{0.1f,0.7f,0.0f,1.0f},
		{0.1f,0.6f,0.0f,1.0f},
		{0.1f,0.5f,0.0f,1.0f},
		{0.1f,0.4f,0.0f,1.0f},
		{0.1f,0.3f,0.0f,1.0f},
		{0.1f,0.2f,0.0f,1.0f},
		{0.1f,0.1f,0.0f,1.0f},
		{0.1f,0.0f,0.0f,1.0f},
	};

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームループ
			directX->PreView();
			for (int i = 0; i < 10; i++) {
				drawEngine[i]->Draw(Left[i], Top[i], Right[i]);
			}
			directX->PostView();
		}
	}
	for (int i = 0; i < 10; i++) {
		drawEngine[i]->Release();
	}
	
	directX->Release();
}