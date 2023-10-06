#include "ImGuiManager.h"


//コンストラクタ
ImGuiManager::ImGuiManager() {
	
	
}

ImGuiManager* ImGuiManager::GetInstance() {
	//関数内static変数として宣言する
	static ImGuiManager instance;

	return &instance;
}



//メインループ前に
//初期化
void ImGuiManager::Initialize(WinApp* winSetup,DirectXSetup* directXSetup) {
	//Getterを使いたい
	//this->winSetup_ = winSetup;
	//this->directXSetup_ = directXSetup;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winSetup->GetHwnd());
	ImGui_ImplDX12_Init(
		directXSetup->GetDevice(),
		directXSetup->GetswapChainDesc().BufferCount,
		directXSetup->GetRtvDesc().Format,
		directXSetup->GetSrvDescriptorHeap(),
		directXSetup->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		directXSetup->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

}

//ここからフレームが始まる
void ImGuiManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}


//更新
//パラメータを変更したいタイミングで処理を行う
void ImGuiManager::UpDate() {
	//これはデモ用のUI
	//使う予定が無いのでコメントアウトします
	//ImGui::ShowDemoWindow();
}


//描画処理前
void ImGuiManager::PreDraw() {
	ImGui::Render();
}

//描画
void ImGuiManager::Draw(DirectXSetup* directXSetup) {
	//描画用のDescriptorの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { directXSetup->GetSrvDescriptorHeap() };
	directXSetup->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}


//ここでフレームが終わる
void ImGuiManager::EndFrame(DirectXSetup* directXSetup) {
	//コマンドを積む
	//実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXSetup->GetCommandList());
}


//デストラクタ
void ImGuiManager::Release() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

ImGuiManager::~ImGuiManager() {

}