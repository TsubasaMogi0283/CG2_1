#include "Player.h"
#include <Collider/CollisionConfig.h>
#include <VectorCalculation.h>

//コンストラクタ
Player::Player() {

}

//初期化
void Player::Initialize() {

	model_ = std::unique_ptr<Model>();
	model_.reset(Model::Create("Resources/Sample/Player", "playre.obj"));

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translate_ = { 0.0f,0.0f,0.0f };

	

	radius_ = 1.0f;
	input_ = Input::GetInstance();

	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY);
}

void Player::OnCollision(){

}


void Player::Rotate() {
	if (input_->IsPushKey(DIK_W) == true) {
		worldTransform_.rotate_.z += ROTATE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_S) == true) {
		worldTransform_.rotate_.z -= ROTATE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_D) == true) {
		worldTransform_.rotate_.y += ROTATE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_A) == true) {
		worldTransform_.rotate_.y -= ROTATE_AMOUNT_;
	}
}

void Player::Move() {
	if (input_->IsPushKey(DIK_UP) == true) {
		worldTransform_.translate_.y += MOVE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_DOWN) == true) {
		worldTransform_.translate_.y -= MOVE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_RIGHT) == true) {
		worldTransform_.translate_.x += MOVE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_LEFT) == true) {
		worldTransform_.translate_.x -= MOVE_AMOUNT_;
	}

	const float MOVE_LIMIT_X = 17.0f;
	const float MOVE_LIMIT_Y = 7.0f;

	worldTransform_.translate_.x = max(worldTransform_.translate_.x, -MOVE_LIMIT_X);
	worldTransform_.translate_.x = min(worldTransform_.translate_.x, MOVE_LIMIT_X);
	worldTransform_.translate_.y = max(worldTransform_.translate_.y, -MOVE_LIMIT_Y);
	worldTransform_.translate_.y = min(worldTransform_.translate_.y, MOVE_LIMIT_Y);

	//資料にはMatrix沢山あるけど
	//Modelで勝手に計算してくれるようにしているから気にしないでね

}


void Player::Attack() {
	if (input_->IsTriggerKey(DIK_SPACE)) {
		

		Vector3 velocity = { 0.0f,0.0f,0.8f };

		
		//Matrix4x4 worldmatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

		//プレイヤーの向きに合わせて回転させる
		velocity = TransformNormal(velocity,worldTransform_.matWorld_ );

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(worldTransform_.translate_,velocity);
		
		bullets_.push_back(newBullet);
	}
}



Vector3 Player::GetWorldPosition() {
	Vector3 result = {};
	//移動成分を取り出してね
	//一番下の行ね
	result.x = worldTransform_.matWorld_.m[3][0];
	result.y = worldTransform_.matWorld_.m[3][1];
	result.z = worldTransform_.matWorld_.m[3][2];

	return result;
}

//更新
void Player::Update() {

	model_->SetColor(color_);
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Scale", &worldTransform_.scale_.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &worldTransform_.rotate_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &worldTransform_.translate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat4("Color", &color_.x, 0.0f, 1.0f);
	ImGui::End();

	model_->SetScale(worldTransform_.scale_);
	model_->SetRotate(worldTransform_.rotate_);
	model_->SetTranslate(worldTransform_.translate_);


	worldTransform_.Update();


	//デスフラグの立った玉を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Rotate();
	Move();
	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

}

//描画
void Player::Draw() {
	
	model_->Draw(worldTransform_);
	
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();
	}
	
}


//デストラクタ
Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

