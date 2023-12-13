#pragma once

#include "Collider.h"
#include <list>

class CollisionManager{
public:
	//コンストラクタ
	CollisionManager();

	//リストをクリアする
	void ClearList();

	//リストに登録する
	void RegisterList(Collider* collider);


	

	//メインの当たり判定処理
	void CheckAllCollision();


	
	//デストラクタ
	~CollisionManager();

private:
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private:
	std::list<Collider*>colliders_;
};

