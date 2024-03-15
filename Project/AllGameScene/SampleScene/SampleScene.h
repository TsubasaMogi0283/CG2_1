#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "Transform.h"

#include "Particle3D.h"
#include "Audio.h"
#include <memory>
#include <Model.h>
#include <numbers>
#include <Object/Player/Player.h>

//StatePatternを使う時は必ず前方宣言をするように
class GameManager;

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
	static const int MODEL_AMOUNT_ = 2;

	std::unique_ptr<Model> model_[MODEL_AMOUNT_] = { nullptr };
	WorldTransform modelWorldTransform_[MODEL_AMOUNT_] = {};

	Vector2 position_ = { 0.0f,0.0f };
	Vector2 scale_ = { 1.0f,1.0f };
	float rotate = 0.0f;
	float intensity_ = 3.0f;
	Vector3 lightingDirection_ = { 0.0f,-1.0f,0.0f };
	Vector3 modelScale_ = { 3.0f,3.0f,3.0f };
	Vector3 pointLightPosition_ = { 0.0f,3.0f,0.0f };
	Vector4 pointLightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	float pointLightRadius_ = 6.0f;
	float pointLightDecay_ = 0.0f;

	Vector2 move = {1.0f,1.0f};



	//
	Vector4 spotLightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	Vector3 spotLightPosition_ = { 2.0f,1.25f,0.0f };
	float spotLightDistance_ = 7.0f;
	Vector3 spotLightDirection_ = { -1.0f,1.0f,0.0f };
	float spotLightIntensity_ = 4.0f;
	float spotLightDecay_ = 2.0f;
	float cosAngle_ = std::cos(std::numbers::pi_v<float> / 3.0f);
	float cosFalloffStart_ = 0.2f;

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


	std::unique_ptr<Particle3D> particle2_ = nullptr;
	uint32_t particleTextureHandle2_ = 0u;
	Vector3 particleTranslate2_ = {};
	AccelerationField accelerationField2_ = {};
	bool isSetField2_ = false;



	std::unique_ptr<Player> player_ = nullptr;



	Camera camera_ = {};
	Vector3 cameraTranslate_ = {0.0f,0.0f,0.0f};
	

	Audio* audio_ = nullptr;
	uint32_t audioHandle2_ = 0u;
	bool isEnd_ = false;
};




