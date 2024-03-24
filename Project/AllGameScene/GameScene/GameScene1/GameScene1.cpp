#include "GameScene1.h"
#include "Ready/GameScene1Ready.h"

GameScene1::GameScene1(){

}

void GameScene1::Initialize(){


	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();


	//currentGamaScene1_ = new GameScene1Ready();
	//currentGamaScene1_->Initialize();
}

void GameScene1::Update(GameManager* gameManager){

	ImGui::Begin("GameScene1");
	ImGui::InputInt("No", &sceneNo_);
	ImGui::End();


	if (sceneNo_ == 0) {
		startTime_ += 1;
		if (startTime_ > 0 && startTime_ <= 120) {
			ImGui::Begin("Ready");
			ImGui::End();

		}
		if (startTime_ > 120 && startTime_ <= 240) {
			ImGui::Begin("Go");
			ImGui::End();

		}
	}
	if (sceneNo_ == 1) {

	}



	skydome_->Update();
	//currentGamaScene1_->Update(this);
}

void GameScene1::Draw(){

	skydome_->Draw();

	//currentGamaScene1_->Draw();
}

GameScene1::~GameScene1(){

}
