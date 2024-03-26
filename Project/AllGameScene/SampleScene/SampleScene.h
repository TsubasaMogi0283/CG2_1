#pragma once
#include "IGameScene.h"

#include "Sprite.h"
#include "Transform.h"

#include "Particle3D.h"
#include "Audio.h"
#include <memory>
#include <Model.h>
#include <numbers>

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

#pragma region モデル

	std::unique_ptr<Model> model1_ = nullptr;
	WorldTransform worldTransform1_ = {};

	std::unique_ptr<Model> model2_ = nullptr;
	WorldTransform worldTransform2_ = {};

#pragma endregion

#pragma region スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

#pragma endregion

	Camera camera_ = {};
	Vector3 cameraTranslate_ = {0.0f,0.0f,0.0f};
	

	Audio* audio_ = nullptr;
	uint32_t audioHandle2_ = 0u;
	bool isEnd_ = false;
};




