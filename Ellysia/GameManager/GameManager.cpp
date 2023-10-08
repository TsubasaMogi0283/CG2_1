#include "GameManager.h"



GameManager::GameManager() {

}
	
void GameManager::Initialize() {

}

void GameManager::BeginFrame() {
	directXSetup_->BeginFrame();
	imGuiManager_->BeginFrame();

}

void GameManager::Update() {

}

void GameManager::Draw() {
	imGuiManager_->PreDraw();	
	imGuiManager_->Draw();
	
	//viewMatrixとprojectionを消したい
	plane2_->Draw(transformModel2,viewMatrix,projectionMatrix);
	plane_->Draw(transformModel,viewMatrix,projectionMatrix);
	

}

void GameManager::EndFrame() {

	imGuiManager_->EndFrame();
	directXSetup_->EndFrame();
			
}

void GameManager::Release() {

}


void GameManager::Operate() {
	//COMの初期化
	//COM...ComponentObjectModel、Microsoftの提唱する設計技術の１つ
	//		DirectX12も簡略化されたCOM(Nano-COM)という設計で作られている
	
	//COMを使用して開発されたソフトウェア部品をCOMコンポーネントと呼ぶ
	//Textureを読むにあたって、COMコンポーネントの１つを利用する
	CoInitializeEx(0, COINIT_MULTITHREADED);

	
	//ここでタイトルバーの名前を決めてね
	const wchar_t* titleBarName = L"Ellysia";

	//ウィンドウのサイズを決める
	const int32_t WINDOW_SIZE_WIDTH = 1280;
	const int32_t WINDOW_SIZE_HEIGHT = 720;


	//コンストラクタ
	//GameManager.cpp側ではwinApp=GetInstanceでやってるけど他のクラスでは直接GetInstanceってやった方が楽かも

	winApp_ = WinApp::GetInstance();
	directXSetup_ = DirectXSetup::GetInstance();
	imGuiManager_ = ImGuiManager::GetInstance();
	input_ = Input::GetInstance();
	

	//初期化
	winApp_->Initialize(titleBarName,WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT);
	directXSetup_->Initialize();
	imGuiManager_->Initialize();
	input_->Initialize();
	



	plane_ = new Model();
	plane_->CreateObject("Resources/05_02", "axis.obj");
	plane_->LoadTexture("Resources/uvChecker.png");
	//
	
	plane2_ = new Model();
	plane2_->CreateObject("Resources/05_02", "plane.obj");
	plane2_->LoadTexture("Resources/uvChecker.png");

	
	


	Transform cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-9.8f} };
	
	
	Transform transformSphere2 = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,2.0f} };

	Vector4 transparency = { 1.0f,1.0f,1.0f,1.0f };
	float transparency2 = 1.0f;
	transformModel = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	transformModel2= { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };



	Sprite* sprite_ = new Sprite();
	//sprite_->Initialize();
	sprite_->LoadTexture("Resources/uvChecker.png");

	transformSprite_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	spriteAllPosition_ = { {0.0f,0.0f},{0.0f,512.0f},{512.0f,0.0f},{512.0f,512.0f} };
	sprite_->SetAllPosition(spriteAllPosition_);


	Sprite* sprite2_ = new Sprite();
	//sprite2_->Initialize();
	sprite2_->LoadTexture("Resources/uvChecker.png");

	transformSprite2_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,0.0f,510.0f} };
	spriteAllPosition2_ = { {0.0f,0.0f},{0.0f,512.0f},{512.0f,0.0f},{512.0f,512.0f} };
	sprite2_->SetAllPosition(spriteAllPosition2_);


	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			//common_->WinMSG(msg);
			winApp_->WindowsMSG(msg);

		}
		else {
			//ゲームの処理
			// 
			//フレームの開始
			BeginFrame();

			#pragma region 更新処理

			imGuiManager_->UpDate();

			//入力の更新
			input_->Update();




			//カメラ行列
			cameraMatrix_ = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			viewMatrix = Inverse(cameraMatrix_);

			//遠視投影行列
			projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WINDOW_SIZE_WIDTH) / float(WINDOW_SIZE_HEIGHT), 0.1f, 100.0f);
			

#pragma region Modelの位置情報

			plane_->SetColor(transparency);
			plane2_->SetTransparency(transparency2);

			//ImGui::Begin("Model");
			//
			//ImGui::InputFloat4("Transparency", &transparency.x);
			//ImGui::SliderFloat4("TransparencySlide", &transparency.x,0.0f,1.0f);
			//
			//
			//ImGui::InputFloat3("Scale", &transformModel.scale.x);
			//ImGui::SliderFloat3("ScaleSlide", &transformModel.scale.x, 1.0f, 5.0f);
			//
			//ImGui::InputFloat3("Rotate", &transformModel.rotate.x);
			//ImGui::SliderFloat3("RotateSlide", &transformModel.rotate.x, 0.0f, 12.0f);
			//
			//ImGui::InputFloat3("Translate", &transformModel.translate.x);
			//ImGui::SliderFloat3("TranslateSlide", &transformModel.translate.x, -10.0f, 10.0f);
			//
			//
			//ImGui::End();
			//
			//
			//
			//
			//ImGui::Begin("Mode2:");
			//
			//
			//ImGui::InputFloat("Transparency", &transparency2);
			//ImGui::SliderFloat("TransparencySlide", &transparency2,0.0f,1.0f);
			//
			//
			//ImGui::InputFloat3("Scale", &transformModel2.scale.x);
			//ImGui::SliderFloat3("ScaleSlide", &transformModel2.scale.x, 1.0f, 5.0f);
			//
			//ImGui::InputFloat3("Rotate", &transformModel2.rotate.x);
			//ImGui::SliderFloat3("RotateSlide", &transformModel2.rotate.x, 0.0f, 12.0f);
			//
			//ImGui::InputFloat3("Translate", &transformModel2.translate.x);
			//ImGui::SliderFloat3("TranslateSlide", &transformModel2.translate.x, -10.0f, 10.0f);
			//
			//
			//ImGui::End();
			ImGui::Begin("Sprite");


			ImGui::InputFloat3("position", &transformSprite_.translate.x);
			ImGui::SliderFloat3("position", &transformSprite_.translate.x, 0.0f, 2000.0f);

			ImGui::End();
			
			ImGui::Begin("Sprite2");

			ImGui::InputFloat3("position", &transformSprite2_.translate.x);
			ImGui::SliderFloat3("position", &transformSprite2_.translate.x, 0.0f, 2000.0f);


			ImGui::End();




#pragma endregion
			#pragma endregion
			Draw();
			
			sprite_->DrawRect(transformSprite_);
			sprite2_->DrawRect(transformSprite2_);


			//フレームの終わり
			EndFrame();

			
		}
		
	}


	//解放処理
	plane_->Release();
	plane2_->Release();
	sprite_->Release();

	delete plane_;
	delete plane2_;
	delete sprite_;
	delete sprite2_;

	directXSetup_->Release();
	winApp_->Close();
	imGuiManager_->Release();

	directXSetup_->DeleteInstance();

	//ゲーム終了時にはCOMの終了処理を行っておく
	CoUninitialize();
}






//デストラクタ
GameManager::~GameManager() {

}


