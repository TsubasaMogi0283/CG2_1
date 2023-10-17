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
#include "Camera/Camera.h"
#include "TextureManager/TextureManager.h"
#include "PipelineManager/PipelineManager.h"




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

	PipelineManager* pipelineManager_ = nullptr;
	ImGuiManager* imGuiManager_ = nullptr;
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	Audio* audio_ = nullptr;

#pragma region サンプル
	


#pragma endregion

	//StatePatternに必要な変数
	IGameScene* currentGamaScene_ = nullptr;



	
};