#include "DebugCamera.h"


//コンストラクタ
DebugCamera::DebugCamera() {

}

//デストラクタ
DebugCamera::~DebugCamera() {

}


//インスタンスを取得
DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;
	return &instance;
}


//初期化
void DebugCamera::Initialize() {

	scale_ = {1.0f,1.0f,1.0f};
	//X,Y,Z軸回りのローカル回転角
	rotattion_ = { 0.0f,0.0f,0.0f };
	//ローカル座標
	translate_ = { 0.0f,0.0f, -50.0f };

	matRot_ = MakeIdentity4x4();

	worldTransform_ = {scale_,rotattion_,translate_ };


	input_ = Input::GetInstance();

}

//更新
void DebugCamera::Update() {

	Vector3 offset = move_;
	offset = TransformNormal(offset, matRot_);
	worldTransform_.translation_ = offset;

	//座標から平行移動行列を計算する
	Matrix4x4 translateMatrix = MakeTranslateMatrix(worldTransform_.translation_);
	//ワールド行列を計算
	worldTransform_.matWorld_ = Multiply(matRot_, translateMatrix);


	#pragma region ビュー行列の更新

	viewProjection_.matView_ = Multiply(Inverse(translateMatrix), Inverse(matRot_));
	//プロジェクション行列の計算
	viewProjection_.matProjection_ = MakePerspectiveFovMatrix(
		viewProjection_.fov_, viewProjection_.aspectRatio_, 
		viewProjection_.nearClip_, viewProjection_.farClip_);

	#pragma endregion

#pragma region 移動

	//前移動
	if (input_->IsPushKey(DIK_UP) == true) {
		ImGui::Begin("a");
		ImGui::End();

		//カメラ移動ベクトル
		Vector3 cameraMove = { 0.0f,0.0f,MOVE_SPEED_AMOUNT_ };

		move_ = Add(move_, cameraMove);
	}
	//後ろ移動
	if (input_->IsPushKey(DIK_DOWN) == true) {
		//カメラ移動ベクトル
		Vector3 cameraMove = { 0.0f,0.0f,-MOVE_SPEED_AMOUNT_ };

		move_ = Add(move_, cameraMove);
	}

	//右移動
	if (input_->IsPushKey(DIK_RIGHT) == true) {
		//カメラ移動ベクトル
		Vector3 cameraMove = { MOVE_SPEED_AMOUNT_,0.0f,0.0f };

		move_ = Add(move_, cameraMove);
	}
	//後ろ移動
	if (input_->IsPushKey(DIK_DOWN) == true) {
		//カメラ移動ベクトル
		Vector3 cameraMove = { -MOVE_SPEED_AMOUNT_,0.0f,0.0f};

		move_ = Add(move_, cameraMove);
	}

#pragma endregion


#pragma region 回転
	//X軸
	if (input_->IsPushKey(DIK_W) == true) {
		
		//追加回転分の回転行列を生成
		Matrix4x4 matRotDelta = MakeIdentity4x4();
		matRotDelta = Multiply(matRotDelta, MakeRotateXMatrix(ROTATE_SPEED_AMOUNT));
		//累積の回転行列を合成
		matRot_ = Multiply(matRotDelta, matRot_);
	}

	if (input_->IsPushKey(DIK_S) == true) {
		//追加回転分の回転行列を生成
		Matrix4x4 matRotDelta = MakeIdentity4x4();
		matRotDelta = Multiply(matRotDelta, MakeRotateXMatrix(ROTATE_SPEED_AMOUNT));
		//累積の回転行列を合成
		matRot_ = Multiply(matRotDelta, matRot_);
	}

	//Y軸
	if (input_->IsPushKey(DIK_W) == true) {
		//追加回転分の回転行列を生成
		Matrix4x4 matRotDelta = MakeIdentity4x4();
		matRotDelta = Multiply(matRotDelta, MakeRotateYMatrix(ROTATE_SPEED_AMOUNT));
		//累積の回転行列を合成
		matRot_ = Multiply(matRotDelta, matRot_);
	}

	if (input_->IsPushKey(DIK_S) == true) {
		//追加回転分の回転行列を生成
		Matrix4x4 matRotDelta = MakeIdentity4x4();
		matRotDelta = Multiply(matRotDelta, MakeRotateYMatrix(ROTATE_SPEED_AMOUNT));
		//累積の回転行列を合成
		matRot_ = Multiply(matRotDelta, matRot_);
	}

	//Z軸
	if (input_->IsPushKey(DIK_Z) == true) {
		//追加回転分の回転行列を生成
		Matrix4x4 matRotDelta = MakeIdentity4x4();
		matRotDelta = Multiply(matRotDelta, MakeRotateZMatrix(ROTATE_SPEED_AMOUNT));
		//累積の回転行列を合成
		matRot_ = Multiply(matRotDelta, matRot_);
	}

	if (input_->IsPushKey(DIK_C) == true) {
		//追加回転分の回転行列を生成
		Matrix4x4 matRotDelta = MakeIdentity4x4();
		matRotDelta = Multiply(matRotDelta, MakeRotateZMatrix(ROTATE_SPEED_AMOUNT));
		//累積の回転行列を合成
		matRot_ = Multiply(matRotDelta, matRot_);
	}

#pragma endregion

}

