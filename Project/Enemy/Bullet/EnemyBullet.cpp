#include "EnemyBullet.h"
#include <Math/Vector/Calculation/VectorCalculation.h>
#include <Player/Player.h>

#include <cmath>


EnemyBullet::EnemyBullet(){

}

void EnemyBullet::Initialzie(Vector3 position, Vector3 velocity){
	


	model_ = new Model();
	model_->CreateObject("Resources/Sample/cube", "cube.obj");

	const float SCALE_ = 0.3f;
	transform_.scale = {SCALE_,SCALE_,SCALE_*SCALE_};
	transform_.translate = position;

	velocity_ = velocity;



}

void EnemyBullet::Update(){
	


	transform_.translate = Add(transform_.translate, velocity_);
	model_->SetColor({ 1.0f,0.0f,0.0f,1.0f });


	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	ImGui::Begin("EnemyBullet");
	ImGui::InputFloat3("Translate", &transform_.translate.x);
	ImGui::End();

}

void EnemyBullet::Draw(){
	model_->Draw(transform_);
}

EnemyBullet::~EnemyBullet(){
	delete model_;

}
