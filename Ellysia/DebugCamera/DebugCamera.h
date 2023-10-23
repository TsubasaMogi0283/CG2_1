#pragma once
#include <Math/Vector/Vector3.h>

class DebugCamera {
private:
	//コンストラクタ
	DebugCamera();

	//デストラクタ
	~DebugCamera();



public:
	
	//インスタンスを取得
	static DebugCamera* GetInstance();

	//初期化
	void Initialize();

	//更新
	void Update();



	




private:
	//X,Y,Z軸回りのローカル回転角
	Vector3 rotattion_ = { 0.0f,0.0f,0.0f };
	//ローカル座標
	Vector3 translate_ = { 0.0f,0.0f, - 50.0f };

	//ビュー行列

	//射影行列


};