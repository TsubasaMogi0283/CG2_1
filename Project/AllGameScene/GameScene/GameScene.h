#pragma once
#include "AllGameScene/GameManager/IGameScene.h"

#include "Sprite.h"
#include "SpritePosition.h"
#include "Transform.h"
#include "Model.h"
#include "Audio.h"
#include <Particle3D.h>

#include <memory>
#include <list>
#include "sstream"

#include "Skydome/Skydome.h"
#include "Player/Player.h"
#include "RailCamera/RailCamera.h"
#include "Enemy/Enemy.h"
#include "Collider/CollisionManager.h"
#include <CountDown/CountDown.h>
#include "Audio/IGameSceneAudio.h"
#include <Enemy/Particle/EnemyParticle.h>

class GameManager;

class GameScene : public IGameScene {
public:

	//コンストラクタ
	GameScene();


	/// 初期化
	void Initialize()override;

	//シーンチェンジ
	void ChangeScene(IGameSceneAudio* newGameScene);
	//シーンのナンバーを取得
	int GetSceneNo() {
		return sceneNo_;
	}

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;


	/// デストラクタ
	~GameScene();


private:
	void DebugText();

#pragma region 初期化
	void ExplanationSceneInitialize();
	void ReadySceneInitialize();
	void PlaySceneInitialize();

#pragma endregion

#pragma region 更新
	void ExplanationSceneUpdate();
	void ReadySceneUpdate();
	void PlaySceneUpdate();

	void LoseSceneUpdate();
	void WinSceneUpdate();


	//衝突判定と応答
	void CheckAllCollisions();


	/*void LoadEnemyPopData();
	void UpdateEnemyPopCommands();*/

	//敵発生用関数
	void GenerateEnemy(Vector3 position);





#pragma endregion

#pragma region 描画
	void ExplanationSceneDraw();
	void ReadySceneDraw();
	void PlaySceneDraw();

	void LoseSceneDraw();
	void WinSceneDraw();

#pragma endregion

private:
	enum class Scene {
		//説明
		Explanation,

		//カウント
		Ready,

		//メインのゲーム
		Play,

		//勝ち
		Win,

		//負け
		Lose,

	};

	Scene scene_ = Scene();
	int sceneNo_ = 0;
	//BlackOut
	std::unique_ptr<Sprite> black_ = nullptr;
	float blackTransparency_ = 1.0f;

	bool isFadeIn_ = true;

	//WhiteOut
	std::unique_ptr<Sprite> white_ = nullptr;
	float whiteTransparency_ = 0.0f;


	//カメラ
	Vector3 cameraTranslate_ = {};
	Vector3 cameraRotate_ = {};
	float cameraAccel_ = 0.05f;
	float cameraVelocity_ = 0.0f;
	//共通部分
	const int SECOND_ = 60;

	//天球ポインタ
	std::unique_ptr<Skydome> skydome_ = nullptr;

	//プレイヤー
	Player* player_ = nullptr;
	float move_ = 0.0f;
	const float MOVE_AMOUNT_ = 0.1f;

	float playerRotateY_ = 0.0f;

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_ = nullptr;

	static const int amount_ = 20;
	//Enemy* enemy_ = nullptr;
	Enemy* enemy_[amount_] = { nullptr };


	//Audio
	IGameSceneAudio* currentGamaSceneAudio_ = nullptr;


#pragma region 説明
	static const int EXPLANATION_NUMBER_ = 2;
	std::unique_ptr<Sprite> explamnationSprite[EXPLANATION_NUMBER_] = { nullptr };
	uint32_t explanationTextureHandle_[EXPLANATION_NUMBER_] = { 0u };
	int explanationTextureNumber_ = 0;
#pragma endregion


#pragma region カウントダウン
	static const int COUNTDOWN_NUMBER_ = 4;
	std::unique_ptr <Sprite> countDownSprite[COUNTDOWN_NUMBER_] = { nullptr };
	uint32_t countDownTexture[COUNTDOWN_NUMBER_] = { 0u };
	
	
	int32_t displayNumber_ = 3;
	int32_t countDownTime_ = displayNumber_*SECOND_;


	Audio* countSE_ = nullptr;
	uint32_t countSEHandle_ = 0u;

	Audio* startSE_ = nullptr;
	uint32_t startSEHandle_ = 0u;

#pragma endregion


#pragma region プレイ

	Model* enemyModel_ = nullptr;

	//std::list<Enemy*> enemyes_;


	//敵発生コマンド
	//std::stringstream enemyPopCommands_;

	//待機中フラグ
	bool isWait_;
	//待機タイマー
	int32_t waitingTimer_ = 0;

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	//カウントダウン
	std::unique_ptr<CountDown> countDown_ = nullptr;

	Audio* killSE_ = nullptr;
	uint32_t killSEHandle_ = 0u;
	int killTime_ = 0;
	//パーティクル
	std::list<EnemyParticle*>particle3D_;


#pragma endregion



#pragma region 負け
	float theta_ = 0.0f;

	int loseLodingTime_ = 0;
	bool isLose_ = false;

	//DecideSE
	Audio* damageSE_ = nullptr;
	uint32_t damageSEHandle_ = 0u;
	int damageTime_ = 0;

#pragma endregion


#pragma region 勝ち
	int finishDisplayTime_ = 0;
	int winLoadingTime_ = 0;
	bool isWin_ = false;
	std::unique_ptr<Sprite> finish_ = nullptr;

	Audio* finishSE_ = nullptr;
	uint32_t finishSEHandle_ = 0u;

	//DecideSE
	Audio* clearSE_ = nullptr;
	uint32_t clearSEHandle_ = 0u;
	int clearTime_ = 0;

#pragma endregion


};

