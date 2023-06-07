#include "HeaderCpp/Function.h"
#include "HeaderCpp/WindowsInitialization.h"
#include "HeaderCpp/DirectXInitialization.h"
//includeなどは全部Function.hに入っているよ！


struct  TriangleProperty {
		Vector4 top;
		Vector4 left;
		Vector4 right;
		TriangleVertex vertex;

	};

//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	const int32_t WINDOW_SIZE_WIDTH = 1280;
	const int32_t WINDOW_SIZE_HEIGHT = 720;


	//コンストラクタ
	WindowsInitialization* winSetup = new WindowsInitialization(L"DirectX",WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT);
	DirectXInitialization* directXSetup = new DirectXInitialization();
	
	
	//三角形の情報
	const int32_t TRIANGLE_AMOUNT_MAX = 15;


	

	
	
	
	TriangleProperty TriangleCoodinate[TRIANGLE_AMOUNT_MAX];
	{
		TriangleCoodinate[0] =
		{

			{-0.8f,1.0f,0.0f,1.0f },
			{-1.0f,0.5f,0.0f,1.0f },
			{-0.6f,0.5f,0.0f,1.0f }
		};
		TriangleCoodinate[1] =
		{

			{-0.4f,1.0f,0.0f,1.0f },
			{-0.6f,0.5f,0.0f,1.0f },
			{-0.2f,0.5f,0.0f,1.0f }
		};
		TriangleCoodinate[2] =
		{

			{0.0f,1.0f,0.0f,1.0f },
			{-0.2f,0.5f,0.0f,1.0f },
			{0.2f,0.5f,0.0f,1.0f }
		};
		TriangleCoodinate[3] =
		{

			{0.4f,1.0f,0.0f,1.0f },
			{0.2f,0.5f,0.0f,1.0f },
			{0.6f,0.5f,0.0f,1.0f }
		};
		TriangleCoodinate[4] =
		{

			{0.8f,1.0f,0.0f,1.0f },
			{0.6f,0.5f,0.0f,1.0f },
			{1.0f,0.5f,0.0f,1.0f }
		};

		TriangleCoodinate[5] =
		{

			{-0.8f,0.5f,0.0f,1.0f },
			{-1.0f,0.0f,0.0f,1.0f },
			{-0.6f,0.0f,0.0f,1.0f }
		};
		TriangleCoodinate[6] =
		{

			{-0.4f,0.5f,0.0f,1.0f },
			{-0.6f,0.0f,0.0f,1.0f },
			{-0.2f,0.0f,0.0f,1.0f }
		};
		TriangleCoodinate[7] =
		{

			{0.0f,0.5f,0.0f,1.0f },
			{-0.2f,0.0f,0.0f,1.0f },
			{0.2f,0.0f,0.0f,1.0f }
		};
		TriangleCoodinate[8] =
		{

			{0.4f,0.5f,0.0f,1.0f },
			{0.2f,0.0f,0.0f,1.0f },
			{0.6f,0.0f,0.0f,1.0f }
		};
		TriangleCoodinate[9] =
		{

			{0.8f,0.5f,0.0f,1.0f },
			{0.6f,0.0f,0.0f,1.0f },
			{1.0f,0.0f,0.0f,1.0f }
		};

		TriangleCoodinate[10] =
		{

			{-0.8f,0.0f,0.0f,1.0f },
			{-1.0f,-0.5f,0.0f,1.0f },
			{-0.6f,-0.5f,0.0f,1.0f }
		};
		TriangleCoodinate[11] =
		{

			{-0.4f,0.0f,0.0f,1.0f },
			{-0.6f,-0.5f,0.0f,1.0f },
			{-0.2f,-0.5f,0.0f,1.0f }
		};
		TriangleCoodinate[12] =
		{

			{0.0f,0.0f,0.0f,1.0f },
			{-0.2f,-0.5f,0.0f,1.0f },
			{0.2f,-0.5f,0.0f,1.0f }
		};
		TriangleCoodinate[13] =
		{

			{0.4f,0.0f,0.0f,1.0f },
			{0.2f,-0.5f,0.0f,1.0f },
			{0.6f,-0.5f,0.0f,1.0f }
		};
		TriangleCoodinate[14] =
		{

			{0.8f,0.0f,0.0f,1.0f },
			{0.6f,-0.5f,0.0f,1.0f },
			{1.0f,-0.5f,0.0f,1.0f }
		};

	}




	//初期化
	winSetup->Initialize();
	directXSetup->Initialize(winSetup->GetClientWidth(),winSetup->GetClientHeight(),winSetup->GetHwnd());


	//三角形について
	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		directXSetup->MakeVertexResource(TriangleCoodinate[i].vertex);
	}


	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			//common_->WinMSG(msg);
			winSetup->WindowsMSG(msg);

		}
		else {
			//ゲームの処理




		}
		directXSetup->BeginFlame(winSetup->GetClientWidth(), winSetup->GetClientHeight());


		for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
			//描画処理
			directXSetup->DrawTriangle(
				TriangleCoodinate[i].top, 
				TriangleCoodinate[i].left, 
				TriangleCoodinate[i].right,
				TriangleCoodinate[i].vertex);
		}

		directXSetup->EndFlame();
	}


	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		TriangleCoodinate[i].vertex.resouce->Release();
	}
	


	//解放処理
	directXSetup->Release();
	winSetup->Close();
	directXSetup->CheckRelease();
	

	directXSetup->~DirectXInitialization();
	winSetup->~WindowsInitialization();;

	

	
	


	
	

	return 0;
}


