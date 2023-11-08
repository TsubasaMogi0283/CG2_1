#pragma once

class Enemy;

class IEnemy{
public:

	virtual void Update(Enemy* enemy)=0;


	virtual ~IEnemy() = 0;



};

