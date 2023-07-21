#include "HeaderCpp/WindowsInitialization.h"
#include "HeaderCpp/DirectXInitialization.h"
#include "HeaderCpp/Triangle.h"



//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	const int32_t WINDOW_SIZE_WIDTH = 1280;
	const int32_t WINDOW_SIZE_HEIGHT = 720;
	
	
	////コンストラクタ
	WindowsInitialization* winSetup = new WindowsInitialization();
	DirectXInitialization* directXSetup = new DirectXInitialization();
	
	
	//三角形の情報
	const int32_t TRIANGLE_AMOUNT_MAX = 15;
	
	
	
	Vector4 TriangleCoodinateLeft[TRIANGLE_AMOUNT_MAX] = {
			//left
			//一段目
			{-1.0f,0.5f,0.0f,1.0f },
			{-0.6f,0.5f,0.0f,1.0f },
			{-0.2f,0.5f,0.0f,1.0f },
			{0.2f,0.5f,0.0f,1.0f },
			{0.6f,0.5f,0.0f,1.0f },
			//二段目
			{-1.0f,0.0f,0.0f,1.0f },
			{-0.6f,0.0f,0.0f,1.0f },
			{-0.2f,0.0f,0.0f,1.0f },
			{0.2f,0.0f,0.0f,1.0f },
			{0.6f,0.0f,0.0f,1.0f },
			//三段目
			{-1.0f,-0.5f,0.0f,1.0f },
			{-0.6f,-0.5f,0.0f,1.0f },
			{-0.2f,-0.5f,0.0f,1.0f },
			{0.2f,-0.5f,0.0f,1.0f },
			{0.6f,-0.5f,0.0f,1.0f },
	
		};
	
	Vector4 TriangleCoodinateTop[TRIANGLE_AMOUNT_MAX] = {
			
			//up
			{-0.8f,1.0f,0.0f,1.0f },
			{-0.4f,1.0f,0.0f,1.0f },
			{0.0f,1.0f,0.0f,1.0f },
			{0.4f,1.0f,0.0f,1.0f },
			{0.8f,1.0f,0.0f,1.0f },
	
			//
			{-0.8f,0.5f,0.0f,1.0f },
			{-0.4f,0.5f,0.0f,1.0f },
			{0.0f,0.5f,0.0f,1.0f },
			{0.4f,0.5f,0.0f,1.0f },
			{0.8f,0.5f,0.0f,1.0f },
			//
			{-0.8f,0.0f,0.0f,1.0f },
			{-0.4f,0.0f,0.0f,1.0f },
			{0.0f,0.0f,0.0f,1.0f },
			{0.4f,0.0f,0.0f,1.0f },
			{0.8f,0.0f,0.0f,1.0f },
		};
	
	Vector4 TriangleCoodinateRight[TRIANGLE_AMOUNT_MAX] = {
			//right
			{-0.6f,0.5f,0.0f,1.0f },
			{-0.2f,0.5f,0.0f,1.0f },
			{0.2f,0.5f,0.0f,1.0f },
			{0.6f,0.5f,0.0f,1.0f },
			{1.0f,0.5f,0.0f,1.0f },
			
			{-0.6f,0.0f,0.0f,1.0f },
			{-0.2f,0.0f,0.0f,1.0f },
			{0.2f,0.0f,0.0f,1.0f },
			{0.6f,0.0f,0.0f,1.0f },
			{1.0f,0.0f,0.0f,1.0f },
	
			{-0.6f,-0.5f,0.0f,1.0f },
			{-0.2f,-0.5f,0.0f,1.0f },
			{0.2f,-0.5f,0.0f,1.0f },
			{0.6f,-0.5f,0.0f,1.0f },
			{1.0f,-0.5f,0.0f,1.0f },
	
		};
	
	
	
	Triangle* triangle[TRIANGLE_AMOUNT_MAX];





	//初期化
	winSetup->Initialize(L"DirectX",WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT);
	directXSetup->Initialize(winSetup->GetClientWidth(),winSetup->GetClientHeight(),winSetup->GetHwnd());

	
	//三角形について
	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		triangle[i] = new Triangle();
		triangle[i]->Initialize(directXSetup);
	
	}

	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			winSetup->WindowsMSG(msg);

		}
		else {
			//ゲームの処理
			//directXSetup->BeginFlame(winSetup->GetClientWidth(), winSetup->GetClientHeight());
			directXSetup->BeginFlame();

			for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
				//描画処理
				triangle[i]->Draw(
					TriangleCoodinateLeft[i], 
					TriangleCoodinateTop[i],
					TriangleCoodinateRight[i]
				);


			}
			

			//directXSetup->EndFlame();
			directXSetup->EndFlame();


		}

		

		
	}


	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		triangle[i]->Release();
	}
	


	//解放処理
	directXSetup->Release();
	winSetup->Close();
	directXSetup->CheckRelease();

	//デストラクタ
	//
	directXSetup->~DirectXInitialization();
	winSetup->~WindowsInitialization();



	
	

	return 0;
}


