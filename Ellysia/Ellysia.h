#pragma once
#include "GameManager.h"
#include "WindowsSetup.h"
#include "DirectXSetup.h"

//Ellysia(エリシア・エレシア)
//Aile + Sheer...「自分の名前」と「シア」
//sheer・・・全くの、完全な。混じりけのない、純粋な
//「Ellysia Engine」というように、後ろにEngine加えても自然だと思う

//崩〇3〇dじゃないよ


class Ellysia {
public:
	//シングルトンダメだった
	//コンストラクタ
	Ellysia();
	
	//基本の動作
	void Operate();

	//デストラクタ
	~Ellysia();

private:
	//初期化処理に入れる関数
	void Initialize();

 	//更新処理の中に入る関数
	void BeginFrame();

	//更新
	void Update();

	//描画
	void Draw();

	//描画処理の中に入る関数
	void EndFrame();

	//解放
	void Release();

private:
	//ウィンドウのサイズを決める
	const int WINDOW_SIZE_WIDTH_ = 1280;
	const int WINDOW_SIZE_HEIGHT_ = 720;


	GameManager* gameManager_ = nullptr;


};