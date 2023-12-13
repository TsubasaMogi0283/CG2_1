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

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	radius_ = 1.0f;
	input_ = Input::GetInstance();

	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY);
}

void Player::OnCollision(){

}


void Player::Rotate() {
	if (input_->IsPushKey(DIK_W) == true) {
		transform_.rotate.z += ROTATE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_S) == true) {
		transform_.rotate.z -= ROTATE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_D) == true) {
		transform_.rotate.y += ROTATE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_A) == true) {
		transform_.rotate.y -= ROTATE_AMOUNT_;
	}
}

void Player::Move() {
	if (input_->IsPushKey(DIK_UP) == true) {
		transform_.translate.y += MOVE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_DOWN) == true) {
		transform_.translate.y -= MOVE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_RIGHT) == true) {
		transform_.translate.x += MOVE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_LEFT) == true) {
		transform_.translate.x -= MOVE_AMOUNT_;
	}

	const float MOVE_LIMIT_X = 17.0f;
	const float MOVE_LIMIT_Y = 7.0f;

	transform_.translate.x = max(transform_.translate.x, -MOVE_LIMIT_X);
	transform_.translate.x = min(transform_.translate.x, MOVE_LIMIT_X);
	transform_.translate.y = max(transform_.translate.y, -MOVE_LIMIT_Y);
	transform_.translate.y = min(transform_.translate.y, MOVE_LIMIT_Y);

	//資料にはMatrix沢山あるけど
	//Modelで勝手に計算してくれるようにしているから気にしないでね

}


void Player::Attack() {
	if (input_->IsTriggerKey(DIK_SPACE)) {
		

		Vector3 velocity = { 0.0f,0.0f,0.8f };

		
		Matrix4x4 worldmatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

		//プレイヤーの向きに合わせて回転させる
		velocity = TransformNormal(velocity,worldmatrix );

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(transform_.translate,velocity);
		
		bullets_.push_back(newBullet);
	}
}



Vector3 Player::GetWorldPosition() {
	Vector3 result = {};
	//移動成分を取り出してね
	//一番下の行ね
	result.x = GetWorldMatrix().m[3][0];
	result.y = GetWorldMatrix().m[3][1];
	result.z = GetWorldMatrix().m[3][2];

	return result;
}

//更新
void Player::Update() {
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Scale", &transform_.scale.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &transform_.rotate.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &transform_.translate.x, -10.0f, 10.0f);

	ImGui::End();

	model_->SetScale(transform_.scale);
	model_->SetRotate(transform_.rotate);
	model_->SetTranslate(transform_.translate);


	//transform_.Update();


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
	
	model_->Draw();
	
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
