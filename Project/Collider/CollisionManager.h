#pragma once

#include "Collider.h"
#include <list>

class CollisionManager{
public:
	//コンストラクタ
	CollisionManager();


	//デストラクタ
	~CollisionManager();

private:
	std::list<Collider*>colliders_;
};

