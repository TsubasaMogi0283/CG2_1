#pragma once

#include "Common/Windows/WinApp.h"
#include "Common/DirectX/DirectXSetup.h"
#include "Polygon/Triangle/Triangle.h"
#include <ImGuiManager/ImGuiManager.h>


#include "Math/Matrix/Matrix/Matrix4x4.h"
#include "ConvertFunction/ConvertColor/ColorConvert.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"
#include <Polygon/Sprite/Sprite.h>
#include <Polygon/Sphere/Sphere.h>
#include <Polygon/Model/Model.h>

#include "Audio/Audio.h"
#include <Input/Input.h>
#include "IGameScene.h"

//main.cppにあるものを全部こっちに引っ越しする
class GameManager {
public:
	//シングルトンダメだった
	//コンストラクタ
	GameManager();

	//基本の動作
	void Operate();

	//シーンチェンジ
	void ChangeScene(IGameScene* newGameScene);

	//デストラクタ
	~GameManager();



private:

	void Initialize();

	void BeginFrame();

	void Update();

	void Draw();

	void EndFrame();


	void Release();


private:
	//ウィンドウのサイズを決める
	const int32_t WINDOW_SIZE_WIDTH_ = 1280;
	const int32_t WINDOW_SIZE_HEIGHT_ = 720;




	WinApp* winApp_ = nullptr;
	DirectXSetup* directXSetup_ = nullptr;

	ImGuiManager* imGuiManager_ = nullptr;
	Input* input_ = nullptr;



#pragma region サンプル
	/*Model* plane_ = nullptr;
	Model* plane2_ = nullptr;*/




	Transform transformModel = {};
	Transform transformModel2 = {};
	Matrix4x4 cameraMatrix_ = {};
	Matrix4x4 viewMatrix = {};

	//遠視投影行列
	Matrix4x4 projectionMatrix = {};


	Sprite* sprite_ = nullptr;

	Transform transformSprite_ = {};
	SpritePosition spriteAllPosition_ = {};

	//Sprite* sprite2_ = nullptr;
	Transform transformSprite2_ = {};
	SpritePosition spriteAllPosition2_ = {};

	Audio* audio_ = nullptr;
	SoundData soundData_ = {};

#pragma endregion

	//StatePatternに必要な変数
	IGameScene* currentGamaScene_ = nullptr;

};