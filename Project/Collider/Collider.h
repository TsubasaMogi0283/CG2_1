#pragma once
class Collider{
public:
	//衝突時に呼ばれる関数
	//仮想関数
	virtual void OnCollision();

	//ワールド座標を取得
	//純粋仮想関数
	virtual Vector3 GetWorldPosition() = 0;


	//半径を取得
	float GetRadius() {
		return radius_;
	}
	//半径を設定
	void SetRadius_(float radius) {
		this->radius_ = radius;
	}

private:
	//衝突半径
	float radius_ = 1.0f;

};

