#include "HeaderCpp/Function.h"
#include "HeaderCpp/WindowsInitialization.h"
#include "HeaderCpp/DirectXInitialization.h"
#include "HeaderCpp/DXGIRelease.h"
//includeなどは全部Function.hに入っているよ！

//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;


	WindowsInitialization* window1= new WindowsInitialization(L"DirectX", kClientWidth,kClientHeight);
	DirectXInitialization* directX = new DirectXInitialization();
	DXGIRelease* releaseCheck = new DXGIRelease();

	window1->WindowInitialize();
	

	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	directX->DirectXInitialize(window1->GetClientWidth(),window1->GetClientHeight(),window1->hwnd_);
	


	TriangleProperty TriangleProperty_[Triangle_Property_MAX];
	{
		TriangleProperty_[0] =
		{

			{-0.8f,1.0f,0.0f,1.0f },
			{-1.0f,0.5f,0.0f,1.0f },
			{-0.6f,0.5f,0.0f,1.0f }
		};
		TriangleProperty_[1] =
		{

			{-0.4f,1.0f,0.0f,1.0f },
			{-0.6f,0.5f,0.0f,1.0f },
			{-0.2f,0.5f,0.0f,1.0f }
		};
		TriangleProperty_[2] =
		{

			{0.0f,1.0f,0.0f,1.0f },
			{-0.2f,0.5f,0.0f,1.0f },
			{0.2f,0.5f,0.0f,1.0f }
		};
		TriangleProperty_[3] =
		{

			{0.4f,1.0f,0.0f,1.0f },
			{0.2f,0.5f,0.0f,1.0f },
			{0.6f,0.5f,0.0f,1.0f }
		};
		TriangleProperty_[4] =
		{

			{0.8f,1.0f,0.0f,1.0f },
			{0.6f,0.5f,0.0f,1.0f },
			{1.0f,0.5f,0.0f,1.0f }
		};

		TriangleProperty_[5] =
		{

			{-0.8f,0.5f,0.0f,1.0f },
			{-1.0f,0.0f,0.0f,1.0f },
			{-0.6f,0.0f,0.0f,1.0f }
		};
		TriangleProperty_[6] =
		{

			{-0.4f,0.5f,0.0f,1.0f },
			{-0.6f,0.0f,0.0f,1.0f },
			{-0.2f,0.0f,0.0f,1.0f }
		};
		TriangleProperty_[7] =
		{

			{0.0f,0.5f,0.0f,1.0f },
			{-0.2f,0.0f,0.0f,1.0f },
			{0.2f,0.0f,0.0f,1.0f }
		};
		TriangleProperty_[8] =
		{

			{0.4f,0.5f,0.0f,1.0f },
			{0.2f,0.0f,0.0f,1.0f },
			{0.6f,0.0f,0.0f,1.0f }
		};
		TriangleProperty_[9] =
		{

			{0.8f,0.5f,0.0f,1.0f },
			{0.6f,0.0f,0.0f,1.0f },
			{1.0f,0.0f,0.0f,1.0f }
		};

		TriangleProperty_[10] =
		{

			{-0.8f,0.0f,0.0f,1.0f },
			{-1.0f,-0.5f,0.0f,1.0f },
			{-0.6f,-0.5f,0.0f,1.0f }
		};
		TriangleProperty_[11] =
		{

			{-0.4f,0.0f,0.0f,1.0f },
			{-0.6f,-0.5f,0.0f,1.0f },
			{-0.2f,-0.5f,0.0f,1.0f }
		};
		TriangleProperty_[12] =
		{

			{0.0f,0.0f,0.0f,1.0f },
			{-0.2f,-0.5f,0.0f,1.0f },
			{0.2f,-0.5f,0.0f,1.0f }
		};
		TriangleProperty_[13] =
		{

			{0.4f,0.0f,0.0f,1.0f },
			{0.2f,-0.5f,0.0f,1.0f },
			{0.6f,-0.5f,0.0f,1.0f }
		};
		TriangleProperty_[14] =
		{

			{0.8f,0.0f,0.0f,1.0f },
			{0.6f,-0.5f,0.0f,1.0f },
			{1.0f,-0.5f,0.0f,1.0f }
		};

	}




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


