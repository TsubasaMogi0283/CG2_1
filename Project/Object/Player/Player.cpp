#include "Player.h"
#include <Object/Collider/CollisionConfig.h>
#include <VectorCalculation.h>
#include "SampleScene/SampleScene.h"
#include "WindowsSetup.h"

//コンストラクタ
Player::Player() {

}

//初期化
void Player::Initialize(Vector3 position) {

	model_ = std::unique_ptr<Model>();
	model_.reset(Model::Create("Resources/Sample/Player", "playre.obj"));

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translate_ = position;

	

	radius_ = 1.0f;

	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY);


#pragma region レティクル
	reticleModel_ = std::unique_ptr<Model>();
	reticleModel_.reset(Model::Create("Resources/Sample/cube","cube.obj"));


	worldTransform3DReticle_.Initialize();
	worldTransform3DReticle_.translate_ = Add(position, { 0.0f,0.0f,5.0f });

	//スプライト版
	reticleSprite_ = std::unique_ptr<Sprite>();
	uint32_t texturehandle = TextureManager::GetInstance()->LoadTexture("Resources/Sample/3DReticle/Sign.png");

	reticleSprite_.reset(Sprite::Create(texturehandle, reticlePosition_));
	Vector2 anchorPoint = { 0.5f,0.5f };
	reticleSprite_->SetAnchorPoint(anchorPoint);

#pragma endregion
}

void Player::OnCollision(){

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
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * MOVE_AMOUNT_;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * MOVE_AMOUNT_;

	}

	if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		move.y += MOVE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		move.y -= MOVE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		move.x += MOVE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		move.x -= MOVE_AMOUNT_;
	}

	const float MOVE_LIMIT_X = 17.0f;
	const float MOVE_LIMIT_Y = 7.0f;

	worldTransform_.translate_ = Add(worldTransform_.translate_, move);


	worldTransform_.translate_.x = max(worldTransform_.translate_.x, -MOVE_LIMIT_X);
	worldTransform_.translate_.x = min(worldTransform_.translate_.x, MOVE_LIMIT_X);
	worldTransform_.translate_.y = max(worldTransform_.translate_.y, -MOVE_LIMIT_Y);
	worldTransform_.translate_.y = min(worldTransform_.translate_.y, MOVE_LIMIT_Y);

}


void Player::Attack() {

	//Bボタン
	if (Input::GetInstance()->GetJoystickState(joyState)){
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			triggerButtonBTime += 1;

		}
		if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) {
			triggerButtonBTime = 0;
		}
	}
	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE)|| (triggerButtonBTime == 1)) {
		

		Vector3 velocity = { 0.0f,0.0f,0.8f };


		//自機から照準オブジェクトへのベクトル
		velocity = Subtract(Get3DReticleWorldPosition(), GetWorldPosition());

		const float BULLET_SPEED = 0.8f;
		velocity.x = Normalize(velocity).x * BULLET_SPEED;
		velocity.y = Normalize(velocity).y * BULLET_SPEED;
		velocity.z = Normalize(velocity).z * BULLET_SPEED;
		//プレイヤーの向きに合わせて回転させる
		velocity = TransformNormal(velocity,worldTransform_.worldMatrix_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(GetWorldPosition(), velocity);
		sampleScene_->AddPlayerBullet(newBullet);
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

Vector3 Player::Get3DReticleWorldPosition(){
	Vector3 result = {};

	result.x = worldTransform3DReticle_.worldMatrix_.m[3][0];
	result.y = worldTransform3DReticle_.worldMatrix_.m[3][1];
	result.z = worldTransform3DReticle_.worldMatrix_.m[3][2];


	return result;
}

//更新
void Player::Update(Camera& camera) {
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Scale", &worldTransform_.scale_.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &worldTransform_.rotate_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &worldTransform_.translate_.x, -10.0f, 10.0f);

	ImGui::End();



	
	
	//自機のワールド座標から3Dレティクルのワールド座標を計算

	//自機から3Dレティクルへの距離
	const float DISTANCE_PLAYER_TO_3D_RETICLE = 50.0f;

	//自機から3Dレティクルへのオフセット(Z向き)
	Vector3 offset = { 0.0f, 0.0f, 1.0f };

	//自機のワールド行列の回復を反映
	offset = TransformNormal(offset, worldTransform_.worldMatrix_);

	//ベクトルの長さを整える
	offset.x = Normalize(offset).x * DISTANCE_PLAYER_TO_3D_RETICLE;
	offset.y = Normalize(offset).y * DISTANCE_PLAYER_TO_3D_RETICLE;
	offset.z = Normalize(offset).z * DISTANCE_PLAYER_TO_3D_RETICLE;

	worldTransform_.Update();

	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translate_ = Add(GetWorldPosition(), offset);
	worldTransform3DReticle_.Update();




	//右スティック
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		spritePosition_.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition_.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;

		//スプライトの座標変更を反映
		reticleSprite_->SetPosition(spritePosition_);

	}

	//2Dレティクル
	positionReticle = Get3DReticleWorldPosition();


	Matrix4x4 worldPositionMatrix =
		MakeAffineMatrix({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, Get3DReticleWorldPosition());

	//3Dレティクルのホ－ルド座標から2Dレティクルのスクリーン座標を計算
	//ビューポート
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0,
			float( WindowsSetup::GetInstance()->WindowsSetup::GetClientWidth()), 
			float(WindowsSetup::GetInstance()->WindowsSetup::GetClientHeight()), 0, 1);


	//ビュー行列
	Matrix4x4 matViewProjectionViewport =
		Multiply(camera.viewMatrix_, Multiply(camera.projectionMatrix_, matViewport));

	//ワールド→スクリーン座標変換(3Dから2Dへ)
	positionReticle = TransformScreen(positionReticle, matViewProjectionViewport);

	//スプライトのレティクルに座標設定
	reticleSprite_->SetPosition(Vector2(positionReticle.x, positionReticle.y));






	POINT mousePosition;

	//マウス座標(スクリーン座標)を取得
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WindowsSetup::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);


	//マウス座標を2Dレティクルのスプライトに入れる
	reticleSprite_->SetPosition(Vector2(float(spritePosition_.x), float(spritePosition_.y)));


	//合成行列の逆行列
	//ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV =
		Multiply(camera.viewMatrix_, Multiply(camera.projectionMatrix_, matViewport));
	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//2点のワールド行列
	//スクリーン座標
	Vector3 posNear = Vector3(float(spritePosition_.x), float(spritePosition_.y), 0);
	Vector3 posFar = Vector3(float(spritePosition_.x), float(spritePosition_.y), 1);

	//スクリーン座標系からワールド座標系へ
	posNear = TransformScreen(posNear, matInverseVPV);
	posFar = TransformScreen(posFar, matInverseVPV);


	//3Dレティクルの座標計算
	//マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	//スプライトの現在座標を取得
	//Vector2 spritePosition = sprite2DReticle_->GetPosition();

	////右スティック
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
	//	spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
	//	
	//	//スプライトの座標変更を反映
	//	sprite2DReticle_->SetPosition(spritePosition);
	//
	//}


	//カメラから照準オブジェクトの距離
	const float DISTANCE_TEST_OBJECT = 50.0f;
	worldTransform3DReticle_.translate_.x = posNear.x + mouseDirection.x * DISTANCE_TEST_OBJECT;
	worldTransform3DReticle_.translate_.y = posNear.y + mouseDirection.y * DISTANCE_TEST_OBJECT;
	worldTransform3DReticle_.translate_.z = posNear.z + mouseDirection.z * DISTANCE_TEST_OBJECT;



	worldTransform3DReticle_.Update();
	



	Rotate();
	Move();
	Attack();


	


	

	
	
}

//描画
void Player::Draw(Camera& camera) {
	
	//本体
	model_->Draw(worldTransform_, camera);

	//レティクル
	reticleModel_->Draw(worldTransform3DReticle_, camera);


	reticleSprite_->Draw();

}


//デストラクタ
Player::~Player() {
}

