#pragma once
#include "Model.h"
#include "Sprite.h"
#include "Input.h"
#include "TextureManager.h"
#include "Matrix4x4.h"
#include "Object/Player/Bullet/PlayerBullet.h"

#include <list>
#include "Object/Collider/Collider.h"
#include "WorldTransform.h"
#include "Camera.h"

class SampleScene;

//Colliderを継承
class Player:public Collider{
public:
	//コンストラクタ
	Player();

	//初期化
	void Initialize(Vector3 position);

	//コールバック関数
	void OnCollision()override;

	//更新
	void Update(Camera& camera);

	//描画
	void Draw(Camera& camera);


	//デストラクタ
	~Player();

#pragma region アクセッサ

	void SetSampleScene(SampleScene* sampleScene) {
		this->sampleScene_ = sampleScene;
	}

	float GetRadius() {
		return radius_;
	}
	
	Vector3 GetRotate() {
		return worldTransform_.rotate_;
	}
	Vector3 GetTranslate() {
		return worldTransform_.translate_;
	}
	


	void SetParent(const WorldTransform* parent) {
		this->worldTransform_.parent_ = parent;
	}


	//ワールド座標
	Vector3 GetWorldPosition()override;

	Vector3 Get3DReticleWorldPosition();


#pragma endregion

private:
	void Rotate();
	void Move();
	void Attack();

private:
	

	std::unique_ptr<Model> model_ = nullptr;

	//Transform transform_ = {};
	WorldTransform worldTransform_ = {};
	const float MOVE_AMOUNT_ = 0.1f;
	const float ROTATE_AMOUNT_ = 0.02f;

	float radius_;


	//弾
	//std::list<PlayerBullet*>bullets_;
	SampleScene* sampleScene_ = nullptr;


	//3Dレティクル
	WorldTransform worldTransform3DReticle_;
	std::unique_ptr<Model> reticleModel_ = nullptr;
	std::unique_ptr<Sprite> reticleSprite_ = nullptr;
	Vector2 reticlePosition_ = {};


	XINPUT_STATE joyState;
	int triggerButtonBTime = 0;
	//マウス
	//初期は適当でいいっしょ
	Vector2 spritePosition_ = { 640.0f,360.0f };
	Vector3 positionReticle = {};

};

