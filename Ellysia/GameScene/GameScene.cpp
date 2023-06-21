#include "GameScene.h"


GameScene::GameScene() {
	winSetup = new WindowsInitialization(L"DirectX",WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_);
	directXSetup = new DirectXInitialization();
}

void GameScene::Initialize() {
	//初期化
	winSetup->Initialize();
	directXSetup->Initialize(WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_,winSetup->GetHwnd());


	//三角形の情報
	const int32_t TRIANGLE_AMOUNT_MAX = 15;

	Triangle* triangle[TRIANGLE_AMOUNT_MAX];
	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		triangle[i] = new Triangle();
		triangle[i]->Initialize(directXSetup);

	}

	

	
}

//ウィンドウのメッセージ
void GameScene::WindowMSG(MSG& msg) {
	winSetup->WindowsMSG(msg);
}


//フレームの始め
void GameScene::BeginFlame() {
	directXSetup->BeginFlame();
}

void GameScene::UpDate() {
	
	
}



void GameScene::Draw() {

}


//フレーム終わり
void GameScene::EndFlame() {

}

void GameScene::Release() {

}




GameScene::~GameScene() {

}