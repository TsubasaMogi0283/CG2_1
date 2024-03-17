#pragma once
#include "Model.h"
#include "Input.h"
#include "TextureManager.h"
#include "Matrix4x4.h"
#include "Object/Player/Bullet/PlayerBullet.h"

#include <list>
#include "Object/Collider/Collider.h"
#include "WorldTransform.h"
#include "Audio.h"

//Colliderを継承
class Player:public Collider{
public:
	//コンストラクタ
	Player();

	//初期化
	void Initialize();

	//コールバック関数
	void OnCollision()override;

	//更新
	void Update();

	//描画
	void Draw(Camera &camera);


	//デストラクタ
	~Player();

#pragma region アクセッサ

	float GetRadius() {
		return radius_;
	}
	
	Vector3 GetRotate() {
		return worldTransform_.rotate_;
	}
	Vector3 GetTranslate() {
		return worldTransform_.translate_;
	}
	
	void SetRotate(Vector3 rotate) {
		this->worldTransform_.rotate_ = rotate;
	}

	void SetTranslate(Vector3 translate) {
		this->worldTransform_.translate_ = translate;
	}

	void SetTranslateZ(float translate) {
		this->worldTransform_.translate_.z = translate;
	}


	//ワールド座標
	Vector3 GetWorldPosition()override;


	//弾が発射できるかどうかの関数

	void SetIsEnableAttack(bool isEnableAttack) {
		this->isEnableAttack_ = isEnableAttack;
	}
	void SetIsEnableMove(bool isEnableMove) {
		this->isEnableMove_ = isEnableMove;
	}

	void SetParent(const WorldTransform* parent) {
		worldTransform_.parent_ = parent;
	}

	bool GetIsDead() {
		return isDead_;
	}

	//弾リストを取得
	const std::list<PlayerBullet*> GetBullets()  {
		return bullets_;
	}

	void SetIsAnimation(bool isAnimation) {
		this->isAnimation_ = isAnimation;
	}



#pragma endregion

private:
	void Rotate();
	void Move();
	void Attack();

private:
	
	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform worldTransform_ = {};
	const float MOVE_AMOUNT_ = 0.1f;
	const float ROTATE_AMOUNT_ = 0.02f;

	float radius_;

	//簡易アニメーション
	bool isAnimation_ = false;
	float playerMoveTheta_ = 0.0f;
	Vector3 animationMove_ = {};

	Matrix4x4 worldMatrix_ = {};
	const WorldTransform* parentedMatrix_ = {};
	//弾
	std::list<PlayerBullet*>bullets_;

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	bool isDead_ = false;

	//弾
	bool isEnableAttack_ = false;
	bool isEnableMove_ = false;


	//ボタン
	XINPUT_STATE joyState_;
	int triggerButtonTime_=0;


	//DecideSE
	Audio* attackSE_ = nullptr;
	uint32_t attackSEHandle_ = 0u;

};

