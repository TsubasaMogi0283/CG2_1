#pragma once
#include <cstdint>
#include "Common/Windows/WindowsInitialization.h"
#include "Common/DirectX/DirectXInitialization.h"

class GameScene {
public:
	GameScene();

	void UpDate();



	void Draw();




	void Release();




	~GameScene();


private:
	const int32_t WINDOW_SIZE_WIDTH_ = 1280;
	const int32_t WINDOW_SIZE_HEIGHT_ = 720;




	WindowsInitialization* winSetup = nullptr;
	DirectXInitialization* directXSetup = nullptr;

};