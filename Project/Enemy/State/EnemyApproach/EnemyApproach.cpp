#include "EnemyApproach.h"
#include "Enemy/Enemy.h"
#include "Enemy/State/EnemyLeave/EnemyLeave.h"

void EnemyApproach::Update(Enemy* enemy){
	Vector3 velocity_ = { 0.0f,0.0f,-0.03f };
	enemy->SetTranslate(Add(enemy->GetTranslate(), velocity_));

	if (enemy->GetTranslate().z < 0.0f) {
		enemy->ChangeState(new EnemyLeave());
	}

}


