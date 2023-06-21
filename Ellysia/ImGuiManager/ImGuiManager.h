#pragma once
#include "Common/Windows/WindowsInitialization.h"
#include "Common/DirectX/DirectXInitialization.h"

class ImGuiManager {
public:

	//コンストラクタ
	ImGuiManager();

	//初期化
	void Initialize(WindowsInitialization* winSetup,DirectXInitialization* directXSetup);

	//ここからフレームが始まる
	void BeginFrame();

	//更新
	void UpDate();


	//描画前の処理
	void PreDraw();

	//描画
	void Draw(DirectXInitialization* directXSetup);


	void EndFrame(DirectXInitialization* directXSetup);


	//デストラクタ
	~ImGuiManager();

	

private:
	//WindowsInitialization* winSetup_ = nullptr;
	//DirectXInitialization* directXSetup_ = nullptr;

	//ID3D12DescriptorHeap* descriptorHeaps_[];
};