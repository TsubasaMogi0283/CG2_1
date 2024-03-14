#pragma once

class Enemy;

class IEnemy{
public:

	virtual void Update(Enemy* enemy)=0;

	virtual void Draw(Enemy* enemy) = 0;

	~IEnemy();


private:



protected:
};

