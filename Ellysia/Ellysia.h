#pragma once

#include "Common/Windows/WinApp.h"
#include "Common/DirectX/DirectXSetup.h"


//・Ellysia(エリシア・エレシア)
//Aile + Sheer...自分の名前」と「シア」
//sheer・・・全くの、完全な。混じりけのない、純粋な
//そこにEngine加えても自然だと思う


//KAMATA ENGINEのGameScene代わりのやつ

class Ellysia {
public:
	//コンストラクタ
	Ellysia();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	//解放
	void Release();

	//デストラクタ
	~Ellysia();

private:
	//初期化処理に入れる関数
	void InitializeCOM();


 	//更新処理の中に入る関数
	void BeginFlame();



	//描画処理の中に入る関数
	void EndFlame();

private:
	//ウィンドウのサイズを決める
	const int32_t WINDOW_SIZE_WIDTH_ = 1280;
	const int32_t WINDOW_SIZE_HEIGHT_ = 720;





};