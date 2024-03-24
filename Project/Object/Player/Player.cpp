#include "Player.h"
#include "Object/Collider/CollisionConfig.h"
#include <VectorCalculation.h>
#include <numbers>
//コンストラクタ
Player::Player() {

}

//初期化
void Player::Initialize() {

	model_ = std::unique_ptr<Model>();
	uint32_t model1 = ModelManager::GetInstance()->LoadObject("Resources/GameResources/Player", "Player.obj");
	model_.reset(Model::Create(model1));
	
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translate_ = { 0.0f,0.0f,0.0f };

	isDead_ = false;

	radius_ = 0.8f;

	//DecideSE
	//attackSE_ = Audio::GetInstance();
	//attackSEHandle_ = attackSE_->LoadWave("Resources/Audio/Action/Attack.wav");



	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY);
}

void Player::OnCollision(){
	isDead_ = true;
}


void Player::Rotate() {
	if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		worldTransform_.rotate_.z += ROTATE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
		worldTransform_.rotate_.z -= ROTATE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
		worldTransform_.rotate_.y += ROTATE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
		worldTransform_.rotate_.y -= ROTATE_AMOUNT_;
	}
}

void Player::Move() {
	Vector3 move = { 0.0f,0.0f,0.0f };
	//左スティック
	if (Input::GetInstance()->GetJoystickState(joyState_)) {
		move.x += (float)joyState_.Gamepad.sThumbLX / SHRT_MAX * MOVE_AMOUNT_;
		move.y += (float)joyState_.Gamepad.sThumbLY / SHRT_MAX * MOVE_AMOUNT_;

	}

	if ((Input::GetInstance()->IsPushKey(DIK_UP) == true)|| (Input::GetInstance()->IsPushKey(DIK_W) == true)) {
		move.y += MOVE_AMOUNT_;
	}
	if ((Input::GetInstance()->IsPushKey(DIK_DOWN) == true)|| (Input::GetInstance()->IsPushKey(DIK_S) == true)) {
		move.y -= MOVE_AMOUNT_;
	}
	if ((Input::GetInstance()->IsPushKey(DIK_RIGHT) == true)|| (Input::GetInstance()->IsPushKey(DIK_D) == true)) {
		move.x += MOVE_AMOUNT_;
	}
	if ((Input::GetInstance()->IsPushKey(DIK_LEFT) == true)|| (Input::GetInstance()->IsPushKey(DIK_A) == true)) {
		move.x -= MOVE_AMOUNT_;
	}

	if (isAnimation_ == true) {
		playerMoveTheta_ += 0.1f;
		const float offset = 0.015f;

		animationMove_.y = float(std::sinf(playerMoveTheta_)) * offset;
	}
	


	worldTransform_.translate_ = Add(worldTransform_.translate_, Add(move, animationMove_));

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

	if (isEnableAttack_ == true) {
		if (Input::GetInstance()->GetJoystickState(joyState_)) {
			if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
				triggerButtonTime_ += 1;
			}
		}
		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			triggerButtonTime_ = 0;
		}

		if ((Input::GetInstance()->IsTriggerKey(DIK_SPACE))|| triggerButtonTime_ == 1) {


			Vector3 velocity = { 0.0f,0.0f,-0.8f };


			//Matrix4x4 worldmatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

			//プレイヤーの向きに合わせて回転させる
			velocity = TransformNormal(velocity, worldTransform_.worldMatrix_);

			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(worldTransform_.translate_, velocity);

			bullets_.push_back(newBullet);


			//attackSE_->PlayWave(attackSEHandle_, false);



		}
	}

	
}



Vector3 Player::GetWorldPosition() {
	Vector3 result = {};
	//移動成分を取り出してね
	//一番下の行ね
	result.x = worldTransform_.worldMatrix_.m[3][0];
	result.y = worldTransform_.worldMatrix_.m[3][1];
	result.z = worldTransform_.worldMatrix_.m[3][2];

	return result;
}

//更新
void Player::Update() {

	model_->SetColor(color_);
	worldTransform_.Update();


	//デスフラグの立った玉を削除
	if (isDead_ == false) {
		bullets_.remove_if([](PlayerBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});

		if (isEnableMove_ == true) {
			Move();
		}
		if (isEnableAttack_ == true) {
			Attack();
		}
		for (PlayerBullet* bullet : bullets_) {
			bullet->Update();
		}
	}
	
	

}

//描画
void Player::Draw(Camera& camera) {
	
	if (isDead_ == false) {
		model_->Draw(worldTransform_,camera);

	}
	
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}


}


//デストラクタ
Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

