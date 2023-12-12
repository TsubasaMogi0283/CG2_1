#include "GameManager.h"

#include "WindowsSetup.h"
#include "DirectXSetup.h"

#include "SampleScene/SampleScene.h"

//コンストラクタ
GameManager::GameManager() {
	
	
	//コンストラクタ
	//GameManager.cpp側ではwinApp=GetInstanceでやってるけど他のクラスでは直接GetInstanceってやった方が楽かも

	imGuiManager_ = ImGuiManager::GetInstance();
	input_ = Input::GetInstance();
	camera_ = Camera::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	audio_ = Audio::GetInstance();
	pipelineManager_ = PipelineManager::GetInstance();
}
	
void GameManager::Initialize() {
	//ここでタイトルバーの名前を決めてね
	const wchar_t* titleBarName = L"Ellysia";

	//初期化
	WindowsSetup::GetInstance()->Initialize(titleBarName,WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_);
	DirectXSetup::GetInstance()->Initialize();
	pipelineManager_->GenerateSpritePSO();
	pipelineManager_->GenerateModelPSO();
	imGuiManager_->Initialize();
	input_->Initialize();
	textureManager_->Initilalize();
	audio_->Initialize();

	//シーンごとに動作確認したいときはここを変えてね
	currentGamaScene_ = new SampleScene();
	currentGamaScene_->Initialize(this);

}

void GameManager::BeginFrame() {
	DirectXSetup::GetInstance()->BeginFrame();
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
	
	currentGamaScene_->Draw(this);


}

void GameManager::EndFrame() {

	imGuiManager_->EndFrame();
	DirectXSetup::GetInstance()->EndFrame();
			
}

void GameManager::Release() {

	audio_->Release();
	//
	textureManager_->Release();
	//
	imGuiManager_->Release();
	//
	DirectXSetup::GetInstance()->Release();
	//

	WindowsSetup::GetInstance()->Close();
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
			WindowsSetup::GetInstance()->WindowsMSG(msg);

		}
		else {
			//ゲームの処理
			// 
			//フレームの開始
			BeginFrame();


			
			//更新処理
			Update();


			//描画
			Draw();
		
			//フレームの終わり
			EndFrame();

			
		}
		
	}

	Release();


}


//デストラクタ
GameManager::~GameManager() {
	delete currentGamaScene_;

}
