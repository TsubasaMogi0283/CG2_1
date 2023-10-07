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
	WinApp* winApp_;
	DirectXSetup* directXSetup_;

	ImGuiManager* imGuiManager_;
	Input* input_;

	Model* plane_;





	Transform transformSphere;
	Matrix4x4 viewMatrix;

	//遠視投影行列
	Matrix4x4 projectionMatrix;
};