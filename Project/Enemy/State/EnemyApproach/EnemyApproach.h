#pragma once
#include "Enemy/State/IEnemy/IEnemy.h"
#include "Enemy/Bullet/EnemyBullet.h"

#include <list>
class Enemy;

class EnemyApproach :public IEnemy{
public:
	 void Update(Enemy* enemy)override;

	 void Draw(Enemy* enemy)override;

	 ~EnemyApproach();



private:
	
};

