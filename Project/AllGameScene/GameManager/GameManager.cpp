#include "GameManager.h"

#include "AllGameScene/SampleScene/SampleScene.h"

//コンストラクタ
GameManager::GameManager() {
	//COMの初期化
	//COM...ComponentObjectModel、Microsoftの提唱する設計技術の１つ
	//		DirectX12も簡略化されたCOM(Nano-COM)という設計で作られている
	
	//COMを使用して開発されたソフトウェア部品をCOMコンポーネントと呼ぶ
	//Textureを読むにあたって、COMコンポーネントの１つを利用する
	CoInitializeEx(0, COINIT_MULTITHREADED);

	
	
	//コンストラクタ
	//GameManager.cpp側ではwinApp=GetInstanceでやってるけど他のクラスでは直接GetInstanceってやった方が楽かも

	winApp_ = WinApp::GetInstance();
	directXSetup_ = DirectXSetup::GetInstance();
	imGuiManager_ = ImGuiManager::GetInstance();
	input_ = Input::GetInstance();
	
}
	
void GameManager::Initialize() {
	//ここでタイトルバーの名前を決めてね
	const wchar_t* titleBarName = L"Ellysia";

	//初期化
	winApp_->Initialize(titleBarName,WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_);
	directXSetup_->Initialize();
	imGuiManager_->Initialize();
	input_->Initialize();
	

	//シーンごとに動作確認したいときはここを変えてね
	currentGamaScene_ = new SampleScene();
	currentGamaScene_->Initialize(this);

}

void GameManager::BeginFrame() {
	directXSetup_->BeginFrame();
	imGuiManager_->BeginFrame();

}


void GameManager::Update() {
	imGuiManager_->UpDate();

	//入力の更新
	input_->Update();
	currentGamaScene_->Update(this);
}

void GameManager::Draw() {
	imGuiManager_->PreDraw();	
	imGuiManager_->Draw();
	
	//viewMatrixとprojectionを消したい
	//plane2_->Draw(transformModel2,viewMatrix,projectionMatrix);
	//plane_->Draw(transformModel,viewMatrix,projectionMatrix);
	//
	//sprite_->DrawRect(transformSprite_);
	//sprite2_->DrawRect(transformSprite2_);

	currentGamaScene_->Draw(this);

}

void GameManager::EndFrame() {

	imGuiManager_->EndFrame();
	directXSetup_->EndFrame();
			
}

void GameManager::Release() {
	imGuiManager_->Release();
	directXSetup_->Release();
	directXSetup_->DeleteInstance();
	winApp_->Close();
}

void GameManager::ChangeScene(IGameScene* newGameScene) {
	//一度消してから次のシーンにいく
	delete currentGamaScene_;

	currentGamaScene_ = newGameScene;
	//今は言っているシーンが引数
	currentGamaScene_->Initialize(this);


}


void GameManager::Operate() {
	
	Initialize();
	

	
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

			

			Update();


			////カメラ行列
			//cameraMatrix_ = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			//viewMatrix = Inverse(cameraMatrix_);
			//
			////遠視投影行列
			//projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WINDOW_SIZE_WIDTH_) / float(WINDOW_SIZE_HEIGHT_), 0.1f, 100.0f);
			

#pragma region Modelの位置情報

			
#pragma endregion
			#pragma endregion
			Draw();
			
			


			//フレームの終わり
			EndFrame();

			
		}
		
	}


	//解放処理
	//plane_->Release();
	//plane2_->Release();
	//sprite_->Release();

	

	//delete plane_;
	//delete plane2_;
	//delete sprite_;
	//delete sprite2_;

	Release();

	//ゲーム終了時にはCOMの終了処理を行っておく
	CoUninitialize();
}






//デストラクタ
GameManager::~GameManager() {

}


