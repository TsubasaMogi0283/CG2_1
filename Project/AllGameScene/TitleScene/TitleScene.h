#pragma once
#include "IGameScene.h"

#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Audio.h"
#include "Sprite.h"

#include "Object/Skydome/Skydome.h"
#include "Camera.h"

class GameManager;

class TitleScene : public IGameScene {
public:

	//コンストラクタ
	TitleScene();

	/// デストラクタ
	~TitleScene();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;

private:

	//天球ポインタ
	Skydome* skydome_ = nullptr;


	std::unique_ptr<Model> playerModel_ = nullptr;
	WorldTransform playerWorldTransform_ = {};

	Vector3 playerTranslate_ = {};
	Vector3 cameraRotate_ = {};

	//カメラ
	float theta_ = 0.0f;


	float transparency_ = 1.0f;
	int32_t loadingTime_ = 0;
	std::unique_ptr<Sprite> blackSprite_ = nullptr;

	bool isFadeIn_ = false;
	bool isStart_ = false;

	//タイトル
	Sprite* titleLogoSprite_ = nullptr;
	uint32_t titleLogoTexture = 0u;

	//プレイヤーモデルの動きについて
	bool isPlayerModelMove_ = false;
	uint32_t playerModelMoveTime_ = 0;
	Vector3 playerVelocity_ = {};
	const float accel_ = 0.05f;

	float playerMoveTheta_ = 0.0f;
	float playerModelRotate_ = 0.0f;

	//スタート
	Sprite* start_ = nullptr;
	uint32_t startHandle_ = 0u;
	int flashTime_ = 0;
	int fastFlashTime_ = 0;
	//ボタン
	XINPUT_STATE joyState_;
	int triggerButtonTime_ = 0;

	//BGM
	Audio* bgm_ = nullptr;
	uint32_t bgmHandle_ = 0u;
	//DecideSE
	Audio* decideSE_ = nullptr;
	uint32_t seHandle_ = 0u;


	float initialCamerTranslate_ = 0.0f;
	Camera& camera_ = {};
};

