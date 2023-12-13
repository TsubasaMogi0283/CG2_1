#pragma once
#include "Model.h"
#include "Input.h"
#include "TextureManager.h"
#include "Matrix4x4.h"
#include "Player/Bullet/PlayerBullet.h"

#include <list>
#include "Collider/Collider.h"
#include "WorldTransform.h"


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
	void Draw();


	//デストラクタ
	~Player();

#pragma region アクセッサ

	float GetRadius() {
		return radius_;
	}
	
	Vector3 GetRotate() {
		return transform_.rotate_;
	}
	Vector3 GetTranslate() {
		return transform_.translate_;
	}
	


	//アフィン行列を取得する
	//KamataEngineではWorldTransformだったけど
	//それに苦戦しまくったのでそれぞれのクラスで計算するようにする
	//理解できないまま進めるのは危険すぎる
	
	Matrix4x4& GetWorldMatrix() {
		return worldMatrix_;
	}

	void SetParent(const Matrix4x4* parent) {
		parentedMatrix_ = parent;
	}


	//ワールド座標
	Vector3 GetWorldPosition()override;

	//弾リストを取得
	const std::list<PlayerBullet*> GetBullets()  {
		return bullets_;
	}

#pragma endregion

private:
	void Rotate();
	void Move();
	void Attack();

private:
	
	//Input
	Input* input_ = nullptr;

	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform transform_ = {};
	const float MOVE_AMOUNT_ = 0.1f;
	const float ROTATE_AMOUNT_ = 0.02f;

	float radius_;


	Matrix4x4 worldMatrix_ = {};
	const Matrix4x4* parentedMatrix_ = {};
	//弾
	std::list<PlayerBullet*>bullets_;



};

