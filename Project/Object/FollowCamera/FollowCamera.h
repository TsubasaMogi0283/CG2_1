#pragma once
#include <Camera.h>
#include <WorldTransform.h>

//レールとカメラオブジェクトも２つの役割を持つよ
class FollowCamera{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FollowCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FollowCamera();


public:
	//アクセッサ

	//追従対象をポインタで持つ
	void SetTraget(const WorldTransform* target) {
		this->target_ = target;
	}

	//カメラの情報を取得する関数
	const Camera GetCamera() {
		return camera_;
	}

private:
	//カメラ
	Camera camera_ = {};

	//追従対象
	//「参照用として」借りているよ
	//勝手に書き換えはダメだよ
	const WorldTransform* target_ = nullptr;

};

