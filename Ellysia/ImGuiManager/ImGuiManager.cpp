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
void ImGuiManager::Initialize() {
	//Getterを使いたい
	this->directXSetup_ = DirectXSetup::GetInstance();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(WinApp::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		directXSetup_->GetDevice(),
		directXSetup_->GetswapChainDesc().BufferCount,
		directXSetup_->GetRtvDesc().Format,
		directXSetup_->GetSrvDescriptorHeap(),
		directXSetup_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		directXSetup_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

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
void ImGuiManager::Draw() {
	//描画用のDescriptorの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { directXSetup_->GetSrvDescriptorHeap() };
	directXSetup_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}


//ここでフレームが終わる
void ImGuiManager::EndFrame() {
	//コマンドを積む
	//実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXSetup_->GetCommandList());
}


//デストラクタ
void ImGuiManager::Release() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

ImGuiManager::~ImGuiManager() {

}