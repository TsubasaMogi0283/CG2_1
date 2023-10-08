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

//main.cppにあるものを全部こっちに引っ越しする
class GameManager {
public:
	//シングルトンダメだった
	//コンストラクタ
	GameManager();
	
	

	//基本の動作
	void Operate();


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
	WinApp* winApp_ = nullptr;
	DirectXSetup* directXSetup_ = nullptr;

	ImGuiManager* imGuiManager_ = nullptr;
	Input* input_ = nullptr;

	Model* plane_ = nullptr;
	Model* plane2_ = nullptr;




	Transform transformModel;
	Transform transformModel2;
	Matrix4x4 cameraMatrix_;
	Matrix4x4 viewMatrix;

	//遠視投影行列
	Matrix4x4 projectionMatrix;



	Transform transformSprite_;
	SpritePosition spriteAllPosition_;

	Transform transformSprite2_;
	SpritePosition spriteAllPosition2_;
};