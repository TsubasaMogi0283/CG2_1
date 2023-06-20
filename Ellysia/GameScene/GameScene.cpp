#include "GameScene.h"


GameScene::GameScene() {
	WindowsInitialization* winSetup = new WindowsInitialization(L"DirectX",WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_);
	DirectXInitialization* directXSetup = new DirectXInitialization();
}

void GameScene::UpDate() {

}



void GameScene::Draw() {

}




void GameScene::Release() {

}




GameScene::~GameScene() {

}