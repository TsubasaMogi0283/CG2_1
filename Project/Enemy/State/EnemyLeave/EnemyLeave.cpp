#include "Enemy/State/EnemyLeave/EnemyLeave.h"
#include "Enemy/Enemy.h"

void EnemyLeave::Update(Enemy* enemy){
	Vector3 velocity_ = { 0.05f,0.01f,-0.03f };
	enemy->SetTranslate(Add(enemy->GetTranslate(), velocity_));

}

