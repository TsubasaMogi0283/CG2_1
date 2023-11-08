#pragma once
#include "Enemy/State/IEnemy/IEnemy.h"

class Enemy;

class EnemyApproach :public IEnemy{
public:
	 void Update(Enemy* enemy)override;



private:

	
};

