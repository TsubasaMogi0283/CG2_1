#include "DebugCamera.h"

//コンストラクタ
DebugCamera::DebugCamera() {

}

//デストラクタ
DebugCamera::~DebugCamera() {

}


//インスタンスを取得
DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;
	return &instance;
}


//初期化
void DebugCamera::Initialize() {

}

//更新
void DebugCamera::Update() {

}

