#include "HeaderCpp/WindowsInitialization.h"
#include "HeaderCpp/DirectXInitialization.h"
#include "HeaderCpp/Triangle.h"


#include "HeaderCpp/Vector4.h"
#include "HeaderCpp/ColorAdapter.h"	

//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	
	//ウィンドウのサイズを決める
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
	
	
	


	//頂いた関数で色を決めていく
	Vector4 Color[TRIANGLE_AMOUNT_MAX] = {
		{ ColorAdapter(RED)},//RED
		{ 0.0f,1.0f,0.0f,1.0f },//GREEN
		{ ColorAdapter(BLUE) },//BLUE
		{ ColorAdapter(WHITE)  },
		{ ColorAdapter(PURPLE) },

		{ 0.0f,0.0f,1.0f,1.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		{ ColorAdapter(BLACK) },
		{ 0.0f,0.0f,1.0f,1.0f },
		{ 1.0f,0.0f,0.0f,1.0f },

		{0.6f,0.5f,0.0f,1.0f },
		{ColorAdapter(BLUE) },
		{0.2f,0.5f,0.0f,1.0f },
		{0.6f,0.5f,0.0f,1.0f },
		{ColorAdapter(WHITE)  },

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




		}
		directXSetup->BeginFlame();


		for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
			//描画処理
			triangle[i]->Draw(
				TriangleCoodinateLeft[i], 
				TriangleCoodinateTop[i],
				TriangleCoodinateRight[i],
				Color[i]
			);
		}

		directXSetup->EndFlame();
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

