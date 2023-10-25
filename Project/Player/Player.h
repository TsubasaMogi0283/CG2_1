#pragma once
#include <Polygon/Model/Model.h>
#include <Input/Input.h>
#include <TextureManager/TextureManager.h>


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
	
	//Input
	Input* input_ = nullptr;
	//Texturemanager
	TextureManager* textureManager_ = nullptr;



	Model* model_ = nullptr;

	Transform transform_ = {};

};

