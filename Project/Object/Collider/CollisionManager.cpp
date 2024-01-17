#include "CollisionManager.h"
#include <Math/Vector/Calculation/VectorCalculation.h>

//基本的にはSampleSceneでやっていたことをこっちに引っ越しするだけ
CollisionManager::CollisionManager(){

}

void CollisionManager::RegisterList(Collider* collider){
	//引数から登録
	colliders_.push_back(collider);
}

//コライダー2つの衝突判定と応答
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	//コライダーAのワールド座標を取得
	Vector3 colliderPosA = colliderA->GetWorldPosition();
	
	//コライダーBのワールド座標を取得
	Vector3 colliderPosB = colliderB->GetWorldPosition();
	
	//衝突フィルタリング
	//ビット演算だから&で
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}

	Vector3 collisionDistance = Subtract(colliderPosA, colliderPosB);

	//距離を計算
	float distance = sqrtf((collisionDistance.x * collisionDistance.x) + 
		(collisionDistance.y * collisionDistance.y) + 
		(collisionDistance.z * collisionDistance.z));
	
	//当たった時
	if (distance < colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}

	

}

void CollisionManager::CheckAllCollision(){
	//総当たりの判定
	//総当たりだからそのままだと自機と自弾にも当たり判定が対応しちゃうよ
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		
		//itrAの*は「参照」
		//itrAを全通りcolliderAに登録しているよ
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			CheckCollisionPair(colliderA, colliderB);

		}

	}
}

//試しにclear抜いてみたら動作が凄く重くなった
void CollisionManager::ClearList(){
	colliders_.clear();
}

CollisionManager::~CollisionManager(){
	
}
