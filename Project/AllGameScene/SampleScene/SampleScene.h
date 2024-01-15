#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "Transform.h"

#include "Particle3D.h"
#include "Audio.h"
#include <memory>
#include <Model.h>

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene{
public:

	//コンストラクタ
	SampleScene();

	/// デストラクタ
	~SampleScene();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;

private:
	static const int MODEL_AMOUNT_ = 1;

	Model* model_[MODEL_AMOUNT_] = { nullptr };
	Vector3 modelTranslate_ = {};

	Vector2 position_ = { 0.0f,0.0f };
	Vector2 scale_ = { 1.0f,1.0f };
	float rotate = 0.0f;

	std::unique_ptr<Sprite> sprite = nullptr;
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 modelColor_ = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 spritePosition_ = {};

	//パーティクル
	std::unique_ptr<Particle3D> particle_ = nullptr;
	uint32_t particleTextureHandle_ = 0u;
	Vector3 particleTranslate_ = {};
	AccelerationField accelerationField_ = {};
	bool isSetField_ = false;

	Audio* audio_ = nullptr;
	uint32_t audioHandle_ = 0u;

	Audio* audio2_ = nullptr;
	uint32_t audioHandle2_ = 0u;


	Vector3 cameraRotate_ = {};
};




