#pragma once




#include "Math/Matrix/Matrix/Matrix4x4.h"
#include "ConvertFunction/ConvertColor/ColorConvert.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"

#include "Audio.h"
#include <Input.h>
#include "IGameScene.h"
#include "Camera.h"
#include "TextureManager.h"
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

	



	PipelineManager* pipelineManager_ = nullptr;
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;
	TextureManager* textureManager_ = nullptr;

#pragma region サンプル
	


#pragma endregion

	//StatePatternに必要な変数
	IGameScene* currentGamaScene_ = nullptr;



	
};