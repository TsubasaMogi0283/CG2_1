#include "HeaderCpp/Function.h"
#include "HeaderCpp/WindowsInitialization.h"
#include "HeaderCpp/DirectXInitialization.h"
#include "HeaderCpp/Triangle.h"
//includeなどは全部Function.hに入っているよ！



	

//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	// 色指定サンプル
	const unsigned int WHITE =0xFFFFFFFF; // 白
	const unsigned int BLACK =0x000000FF; // 黒
	const unsigned int RED =0xFF0000FF;   // 赤
	const unsigned int GREEN =0x00FF0FF;  // 緑
	const unsigned int BLUE = 0x0000FFFF; // 青

	const unsigned int  YELLOW = 0xFFFF00FF; //黄色
	const unsigned int PURPLE = 0xFF00FFFF; //紫


	const int32_t WINDOW_SIZE_WIDTH = 1280;
	const int32_t WINDOW_SIZE_HEIGHT = 720;


	//コンストラクタ
	WindowsInitialization* winSetup = new WindowsInitialization(L"DirectX",WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT);
	DirectXInitialization* directXSetup = new DirectXInitialization();
	
	
	
	//三角形の情報
	const int32_t TRIANGLE_AMOUNT_MAX = 15;

	
	

	//初期化
	winSetup->Initialize();
	directXSetup->Initialize(winSetup->GetClientWidth(),winSetup->GetClientHeight(),winSetup->GetHwnd());

	Triangle* triangle[TRIANGLE_AMOUNT_MAX];
	////三角形について
	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		triangle[i] = new Triangle();
		triangle[i]->Initialize(directXSetup);

	}


	

	
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
	
	
	
	//色
	unsigned int  Color[TRIANGLE_AMOUNT_MAX] = {
		{ RED},//RED
		{ GREEN},//GREEN
		{ BLUE },//BLUE
		{ WHITE  },
		{ PURPLE },

		{ BLUE },
		{ RED },
		{ BLACK },
		{ WHITE },
		{ YELLOW },

		{BLACK },
		{BLUE },
		{RED},
		{PURPLE },
		{WHITE  },

	};
	

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
			directXSetup->BeginFlame();


			for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
				//描画処理
				triangle[i]->Draw(
					TriangleCoodinateLeft[i], 
					TriangleCoodinateTop[i],
					TriangleCoodinateRight[i],
					
				);
			}

		directXSetup->EndFlame();



		}
		
	}

	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		//描画処理
		triangle[i]->Release();
		
	}




	
	


	//解放処理
	directXSetup->Release();
	winSetup->Close();

	delete directXSetup;
	delete winSetup;

	
	

	return 0;
}

