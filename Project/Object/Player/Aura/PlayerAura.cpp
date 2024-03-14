#include "PlayerAura.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerAura::PlayerAura(){

}

/// <summary>
/// 初期化
/// </summary>
void PlayerAura::Initialize(Vector3 position){
	model_ = std::make_unique<Model>();
	model_->CreateObj("Resources/PlayerAura", "PlayerAura.obj");

	color_ = { 1.0f,1.0f,1.0f,1.0f };

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	const float SCALE = 0.1f;
	worldTransform_.scale_ = { SCALE,SCALE,SCALE };
	worldTransform_.translate_ = position;
}

/// <summary>
/// 更新
/// </summary>
void PlayerAura::Update(){

	model_->SetColor(color_);


	worldTransform_.Update();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera"></param>
void PlayerAura::Draw(){
	model_->Draw(worldTransform_);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerAura::~PlayerAura(){

}
