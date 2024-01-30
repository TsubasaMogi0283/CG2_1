#include "EnemyApproach.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Enemy/State/EnemyLeave/EnemyLeave.h"

//接近の時だけ弾が発射されるよ
#include "VectorCalculation.h"

void EnemyApproach::Update(Enemy* enemy){

	Vector3 velocity_ = { 0.0f,0.0f,-0.2f };
	enemy->SetTranslate(Add(enemy->GetTranslate(), velocity_));

	//離脱へ
	if (enemy->GetTranslate().z <10.0f) {
		enemy->ChangeState(new EnemyLeave());
	}

	

}


void EnemyApproach::Draw(Enemy* enemy) {
	
}


EnemyApproach::~EnemyApproach() {
	
	
}
