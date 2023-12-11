#pragma once
#include "AllGameScene/GameManager/IGameScene.h"

#include "Polygon/Sprite/Sprite.h"
#include <Math/Vector/SpritePosition.h>
#include "Math/Vector/Transform.h"

#include "Polygon/Particle/3D/Particle3D.h"
#include "Polygon/Model/Model.h"
#include <memory>

//StatePatternをル買う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene();

	/// デストラクタ
	~SampleScene();

	/// 初期化
	void Initialize(GameManager* gameManager)override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw(GameManager* gameManager)override;

private:

	std::unique_ptr< Particle3D> model_ = nullptr;
	Vector4 modelColor_ = {1.0f,1.0f,1.0f,1.0f};

	Vector3 scale_ = { 1.0f,1.0f,1.0f };
	Vector3 traslate_ = {};
	Vector3 velocity_ = {};

	Vector3 cameraPosition_ = {};
	Vector3 cameraRotate_ = {};

	bool isEmit_ = false;

	uint32_t particleTextureHandle_ = 0u;

	Emitter emitter_ = {};

};




