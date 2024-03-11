#include "ParticleEmitter.h"
#include <Polygon/3D/Particle3DManager/Particle3DManager.h>

void ParticleEmitter::Initialize(){
	transform_ = { 0.0f,0.0f,0.0f };
	name_ = {};
	count_ = 10;


	emitter_.count = 100;
	//0.5秒ごとに発生
	emitter_.frequency = 0.5f;
	//発生頻度用の時刻。0.0で初期化
	emitter_.frequencyTime = 0.0f;
	//SRT
	emitter_.transform.scale = { 1.0f,1.0f,1.0f };
	emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter_.transform.translate = { 0.0f,0.0f,0.0f };
}

void ParticleEmitter::Update(){
	///時間経過
	emitter_.frequencyTime += DELTA_TIME;
	//頻度より大きいなら
	if (emitter_.frequency <= emitter_.frequencyTime) {
		
		//ManagerのEmitを呼び出すだけ
		Particle3DManager::GetInstance()->Emit(name_, transform_, count_);

		//余計に過ぎた時間も加味して頻度計算する
		emitter_.frequencyTime -= emitter_.frequency;

	}
}

void ParticleEmitter::Emit(){
	
}
