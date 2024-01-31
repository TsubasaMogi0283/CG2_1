#include "FollowCamera.h"
#include <VectorCalculation.h>

/// <summary>
/// コンストラクタ
/// </summary>
FollowCamera::FollowCamera() {

}

/// <summary>
/// 初期化
/// </summary>
void FollowCamera::Initialize() {
	camera_.Initialize();
	camera_.rotate_ = { 0.0f,0.0f,0.0f };
	camera_.translate_ = { 0.0f,0.0f,0.0f };

}

/// <summary>
/// 更新
/// </summary>
void FollowCamera::Update() {
	//追従対象がいれば
	if (target_) {
		//追従対象からカメラまでのオフセット
		//ちょっと上から見下ろす感じだね
		Vector3 offset = { 0.0f,2.0f,-10.0f };

		//座標をコピーしてオフセット分ずらす
		camera_.translate_ = Add(target_->translate_, offset);

	}


	//カメラの更新
	camera_.Update();


}

/// <summary>
/// デストラクタ
/// </summary>
FollowCamera::~FollowCamera() {

}
