#pragma once
#include <Windows/WindowsApp.h>
#include <DirectX/DirectXSetup.h>
#include "Polygon/Triangle/Triangle.h"
#include <Vector/TriangleCoodinate.h>

class Ellysia {
public:
	Ellysia();

	void Initialize();

	void WindowMsg(MSG msg);

#pragma region ゲームループ
	void BeginFlame();

	void Update();


	void Draw();

	void EndFlame();

#pragma endregion

	void Release();

	~Ellysia();


private:

	const int32_t WINDOW_SIZE_WIDTH_ = 1280;
	const int32_t WINDOW_SIZE_HEIGHT_ = 720;

	//三角形の情報
	static const int32_t TRIANGLE_AMOUNT_MAX = 15;

	WindowsApp* winSetup_ = nullptr;
	DirectXSetup* directXSetup_ =nullptr;
	Triangle* triangle_[TRIANGLE_AMOUNT_MAX];




};