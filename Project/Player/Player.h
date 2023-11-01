#pragma once
#include <Polygon/Model/Model.h>
#include <Input/Input.h>
#include <TextureManager/TextureManager.h>

#include "Player/Bullet/PlayerBullet.h"

class Player{
public:
	//コンストラクタ
	Player();

	//初期化
	void Initialize();

	

	//更新
	void Update();

	//描画
	void Draw();


	//デストラクタ
	~Player();

private:
	void Rotate();
	void Move();
	void Attack();

private:
	
	//Input
	Input* input_ = nullptr;

	Model* model_ = nullptr;

	Transform transform_ = {};
	const float MOVE_AMOUNT_ = 0.1f;
	const float ROTATE_AMOUNT_ = 0.02f;



	PlayerBullet* bullet_ = nullptr;


};

