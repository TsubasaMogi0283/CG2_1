#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){

}

void PlayerBullet::Initialize(Vector3 position){
	//良い感じなの無かったからとりあえずこれで
	//真っ黒
	model_ = new Model();
	model_->CreateObject("Resources/Sample/cube", "cube.obj");
	transform_.translate = position;

}

void PlayerBullet::Update(){




}

void PlayerBullet::Draw(){
	model_->Draw(transform_);
}

PlayerBullet::~PlayerBullet(){
	delete model_;
}
