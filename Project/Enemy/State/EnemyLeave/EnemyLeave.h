#pragma once
#include "Enemy/State/IEnemy/IEnemy.h"

class Enemy;

class EnemyLeave :public IEnemy{
public:

	void Update(Enemy* enemy)override;


private:

	

};

