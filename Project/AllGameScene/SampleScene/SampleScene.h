#pragma once
#include "AllGameScene/GameManager/IGameScene.h"

#include "Sprite.h"
#include "GameManager.h"
#include "Model.h"
#include "TextureManager.h"
#include "Camera.h"

#include <Object/Player/Player.h>
#include <Object/Enemy/Enemy.h>
#include <Object/Skydome/Skydome.h>
#include <Object/Collider/CollisionManager.h>
#include <Object/RailCamera/RailCamera.h>

#include <memory>
#include <Line/Line.h>
#include <Particle3D.h>


//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
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
	std::unique_ptr <Model> sphere = nullptr;
	WorldTransform sphereWorldTransform_ = {};

	std::unique_ptr<Model> terrain_ = nullptr;
	WorldTransform terrainWorldTransform_ = {};

	Vector3 lightPosition_ = {0.0f,2.0f,0.0f};
	float pointLightIntencity_ = 1.0f;

	Vector3 pointLightPosition_ = { 0.0f,2.0f,0.0f };
	float pointLightIntensity_ = 1.0f;


	Particle3D* particle3D_ = nullptr;
	uint32_t textureHandle_ = 0u;


	Camera camera_ = {};
};