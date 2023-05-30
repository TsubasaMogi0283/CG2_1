#include "HeaderCpp/DirectX.h"
#include "HeaderCpp/WinApp.h"
#include "HeaderCpp/DrawEngine.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//ウィンドウクラス
	WinApp* winApp = new WinApp();
	int32_t kClientWidth = 1280;
	int32_t kClientHeight = 720;

	winApp->Initialize(kClientWidth, kClientHeight);
	//DirectX
	DirectX* directX = new DirectX();
	directX->Initialize(winApp, kClientWidth, kClientHeight);
	DrawEngine* drawEngine = new DrawEngine();
	drawEngine->Initialize(directX);
	MSG msg{};

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームループ
			directX->PreView();
			drawEngine->Draw({-0.1f,0.9f,0.0f,1.0f},{0.0f,1.0f,0.0f,1.0f},{0.1f,0.9f,0.0f,1.0f});
			drawEngine->Draw({-0.1f,0.8f,0.0f,1.0f},{0.0f,0.9f,0.0f,1.0f},{0.1f,0.8f,0.0f,1.0f});
			drawEngine->Draw({-0.1f,0.7f,0.0f,1.0f},{0.0f,0.8f,0.0f,1.0f},{0.1f,0.7f,0.0f,1.0f});
			drawEngine->Draw({-0.1f,0.6f,0.0f,1.0f},{0.0f,0.7f,0.0f,1.0f},{0.1f,0.6f,0.0f,1.0f});
			drawEngine->Draw({-0.1f,0.5f,0.0f,1.0f},{0.0f,0.6f,0.0f,1.0f},{0.1f,0.5f,0.0f,1.0f});
			drawEngine->Draw({-0.1f,0.4f,0.0f,1.0f},{0.0f,0.5f,0.0f,1.0f},{0.1f,0.4f,0.0f,1.0f});
			drawEngine->Draw({-0.1f,0.3f,0.0f,1.0f},{0.0f,0.4f,0.0f,1.0f},{0.1f,0.3f,0.0f,1.0f});
			drawEngine->Draw({-0.1f,0.2f,0.0f,1.0f},{0.0f,0.3f,0.0f,1.0f},{0.1f,0.2f,0.0f,1.0f});
			drawEngine->Draw({-0.1f,0.1f,0.0f,1.0f},{0.0f,0.2f,0.0f,1.0f},{0.1f,0.1f,0.0f,1.0f});
			drawEngine->Draw({-0.1f,0.0f,0.0f,1.0f},{0.0f,0.1f,0.0f,1.0f},{0.1f,0.0f,0.0f,1.0f});
			directX->PostView();
		}
	}
	drawEngine->Release();
	directX->Release();
}

