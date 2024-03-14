#pragma once
#include "WorldTransform.h"
#include "Camera.h"
#include "Model.h"
#include "Vector3.h"
#include "Vector4.h"

class PlayerAura{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerAura();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerAura();

public:

	/// <summary>
	/// ポジションを入れるアクセッサ
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position) {
		this->worldTransform_.translate_ = position;
	}

	void SetScale(Vector3 scale) {
		this->worldTransform_.scale_ = scale;
	}


private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_ = {};

	Vector4 color_ = {};
};

