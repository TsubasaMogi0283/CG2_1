#include "Enemy/State/EnemyLeave/EnemyLeave.h"
#include "Enemy/Enemy.h"
#include "VectorCalculation.h"

void EnemyLeave::Update(Enemy* enemy){
	//state_ = 1;
	Vector3 velocity_ = { 0.05f,0.01f,-0.03f };
	enemy->SetTranslate(Add(enemy->GetTranslate(), velocity_));

}

void EnemyLeave::Draw(Enemy* enemy) {

}
