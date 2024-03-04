#pragma once

class Particle3DManager final{
private:
	//コンストラクタ
	Particle3DManager() = default;

	//デストラクタ
	~Particle3DManager() = default;

public:
	//インスタンス
	static Particle3DManager* GetInstance();

	//コピーコンストラクタ禁止
	Particle3DManager(const Particle3DManager& particle3DManager) = delete;

	//代入演算子を無効にする
	Particle3DManager& operator=(const Particle3DManager& particle3DManager) = delete;


public:
	void Initialize();






};

