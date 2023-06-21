#pragma once
#include "Common/Windows/WindowsInitialization.h"
#include "Common/DirectX/DirectXInitialization.h"

class ImGuiManager {
public:

	//コンストラクタ
	ImGuiManager();

	//初期化
	void Initialize(WindowsInitialization* winSetup,DirectXInitialization* directXSetup);



	//更新
	void UpDate();



	//描画
	void Draw();





	//デストラクタ
	~ImGuiManager();

	

private:
	

};