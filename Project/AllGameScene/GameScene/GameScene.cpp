#include "GameScene.h"


GameScene::GameScene() {
	winSetup = WinApp::GetInstance();
	directXSetup = new DirectXSetup;
}

void GameScene::Initialize() {
	//初期化
	winSetup->Initialize(L"DirectX",WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_);
	directXSetup->Initialize(WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_,winSetup->GetHwnd());


	

	
}

//ウィンドウのメッセージ
void GameScene::WindowMSG(MSG& msg) {
	winSetup->WindowsMSG(msg);
}


//フレームの始め
void GameScene::BeginFrame() {
	directXSetup->BeginFrame();
}

void GameScene::UpDate() {
	
	
}



void GameScene::Draw() {

}


//フレーム終わり
void GameScene::EndFrame() {

}

void GameScene::Release() {

}




GameScene::~GameScene() {

}