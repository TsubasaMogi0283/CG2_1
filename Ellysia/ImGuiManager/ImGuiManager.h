#pragma once
#include "Common/Windows/WinApp.h"
#include "Common/DirectX/DirectXSetup.h"

class ImGuiManager {
public:

	//コンストラクタ
	ImGuiManager();

	//初期化
	void Initialize(WinApp* winSetup,DirectXSetup* directXSetup);

	//ここからフレームが始まる
	void BeginFrame();

	//更新
	void UpDate();


	//描画前の処理
	void PreDraw();

	//描画
	void Draw(DirectXSetup* directXSetup);


	void EndFrame(DirectXSetup* directXSetup);


	//解放処理
	void Release();

	

private:
	//WindowsInitialization* winSetup_ = nullptr;
	//DirectXInitialization* directXSetup_ = nullptr;

	//ID3D12DescriptorHeap* descriptorHeaps_[];
};