#include "Ellysia.h"

#include <PipelineManager.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <TextureManager.h>
#include <Audio.h>

Ellysia::Ellysia() {
}


void Ellysia::Initialize(){
	//ここでタイトルバーの名前を決めてね
	const wchar_t* titleBarName = L"Ellysia";

	//初期化
	//ウィンドウ
	WindowsSetup::GetInstance()->Initialize(titleBarName,WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_);
	
	//DirectX
	DirectXSetup::GetInstance()->Initialize();
	
	//ImGuiManager
	ImGuiManager::GetInstance()->Initialize();
	
	//Input
	Input::GetInstance()->Initialize();
	
	//TextureManager
	TextureManager::GetInstance()->Initilalize();
	
	//Audio
	Audio::GetInstance()->Initialize();


	//GameManager
	gameManager_ = new GameManager();
	gameManager_->Initialize();

}


#pragma region ゲームループ内の関数

void Ellysia::BeginFrame(){
	DirectXSetup::GetInstance()->BeginFrame();
	ImGuiManager::GetInstance()->BeginFrame();
}

void Ellysia::Update(){
	ImGuiManager::GetInstance()->Update();

	//入力の更新
	Input::GetInstance()->Update();
	gameManager_->Update();
}

void Ellysia::Draw(){
	ImGuiManager::GetInstance()->PreDraw();	
	ImGuiManager::GetInstance()->Draw();
	gameManager_->Draw();
}


void Ellysia::EndFrame() {

	ImGuiManager::GetInstance()->EndFrame();
	DirectXSetup::GetInstance()->EndFrame();
			
}
#pragma endregion


void Ellysia::Operate(){
	//初期化
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

	//解放
	Release();


}



void Ellysia::Release() {

	Audio::GetInstance()->Release();
	TextureManager::GetInstance()->Release();
	ImGuiManager::GetInstance()->Release();
	DirectXSetup::GetInstance()->Release();
	WindowsSetup::GetInstance()->Close();
}

Ellysia::~Ellysia(){
	delete gameManager_;
}
