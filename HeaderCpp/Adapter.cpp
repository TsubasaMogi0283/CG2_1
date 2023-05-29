#include "Adapter.h"

Adapter::Adapter() {

}

Adapter::~Adapter() {

}


void Adapter::Initialize(const wchar_t* title, const int32_t Width, const int32_t Height) {
	windowsSetup_->WindowInitialize(title,Width, Height);
	
	directXSetup_->MakeDXGIFactory();
	directXSetup_->GenerateD3D12Device();

	//エラー警告
	directXSetup_->StopErrorWarning();


	//コマンド生成
	directXSetup_->MakeCommandList();
	//スワップチェーン
	directXSetup_->MakeSwapChain(Width,Height,windowsSetup_->GetHwnd());
	//RTVDescriptor
	directXSetup_->MakeDescriptorHeap();
	//スアップチェーンを引っ張る
	directXSetup_->PullResourcesFromSwapChain();
	//RTVの生成
	directXSetup_->MakeRTV();
	//DXCの初期化
	directXSetup_->DXCInitialize();
	//PSOの生成
	directXSetup_->MakePSO();

}


void Adapter::WinMSG(MSG& msg) {
	windowsSetup_->WindowProc(windowsSetup_->GetHwnd(),msg,)
}

void Adapter::BeginFlame(const int32_t kClientWidth, const int32_t kClientHeight) {

}

void Adapter::EndFlame() {

}

void Adapter::TriangleDraw(Vector4 top, Vector4 left, Vector4 right) {

}

void Adapter::Deleate() {

}