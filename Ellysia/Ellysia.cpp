#include "Ellysia.h"


Ellysia::Ellysia() {
	winSetup_ = new WindowsApp();
	directXSetup_ = new DirectXSetup();
}

void Ellysia::Initialize() {
	//初期化
	winSetup_->Initialize(L"DirectX",WINDOW_SIZE_WIDTH_,WINDOW_SIZE_HEIGHT_);
	directXSetup_->Initialize(winSetup_->GetClientWidth(),winSetup_->GetClientHeight(),winSetup_->GetHwnd());

	

	//三角形について
	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		triangle_[i] = new Triangle();
		triangle_[i]->Initialize(directXSetup_);

		//左下
		triangle_[i]->SetVertexData0({ -0.2f, 0.8f - 0.2f * i, 0.0f, 1.0f });
		triangle_[i]->SetVertexData1({0.0f,1.0f - 0.2f * i,0.0f,1.0f});
		triangle_[i]->SetVertexData2({  0.2f,0.8f - 0.2f * i,0.0f,1.0f  });
	}
	
}
void Ellysia::WindowMsg(MSG msg) {
	winSetup_->WindowsMSG(msg);
}

void Ellysia::BeginFlame() {
	directXSetup_->BeginFlame();
}





void Ellysia::Update() {
	


	




}

void Ellysia::Draw() {

	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		//描画処理
		triangle_[i]->Draw();


	}
	

}

void Ellysia::EndFlame() {
	directXSetup_->EndFlame();

}

void Ellysia::Release() {
	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		triangle_[i]->Release();
	}
	


	//解放処理
	directXSetup_->Release();
	winSetup_->Close();
	directXSetup_->CheckRelease();
}

Ellysia::~Ellysia() {
	directXSetup_->~DirectXSetup();
	winSetup_->~WindowsApp();

}
