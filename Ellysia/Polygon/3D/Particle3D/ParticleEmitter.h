#pragma once

struct Emitter {
	//エミッタのTransform;
	Transform transform;
	//発生数
	uint32_t count;
	//発生頻度
	float frequency;
	//頻度用時刻
	float frequencyTime;
};

class ParticleEmitter{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleEmitter() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	


	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 発生
	/// </summary>
	void Emit();



	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleEmitter() {};


private:
	//エミッタの設定
	Emitter emitter_ = {};
	const float DELTA_TIME = 1.0f / 60.0f;

	Vector3 transform_ = {};
	std::string name_ = {};
	uint32_t count_ = 10;

};

