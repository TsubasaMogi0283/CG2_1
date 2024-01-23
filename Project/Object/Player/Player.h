#pragma once
#include "Model.h"
#include "Input.h"
#include "TextureManager.h"
#include "Matrix4x4.h"
#include "Object/Player/Bullet/PlayerBullet.h"

#include <list>
#include "Object/Collider/Collider.h"
#include "WorldTransform.h"


//Colliderを継承
class Player{
public:
	//コンストラクタ
	Player();

	//初期化
	void Initialize(Vector3 position);


	//更新
	void Update();

	//描画
	void Draw(Camera& camera);


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
	


	void SetParent(const WorldTransform* parent) {
		this->worldTransform_.parent_ = parent;
	}


	//ワールド座標
	Vector3 GetWorldPosition();


#pragma endregion

private:
	void Rotate();
	void Move();

private:
	

	std::unique_ptr<Model> model_ = nullptr;

	//Transform transform_ = {};
	WorldTransform worldTransform_ = {};
	const float MOVE_AMOUNT_ = 0.1f;
	const float ROTATE_AMOUNT_ = 0.02f;

	float radius_;

	Vector3 directionalLight_ = {};
	float shininess_ = 70.0f;
	uint32_t textureHandle_ = 0u;
};

