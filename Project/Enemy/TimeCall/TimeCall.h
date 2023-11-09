#pragma once
#include <functional>

class TimeCall{
public:
	//コンストラクタ
	TimedCall(std::function<void()> f, uint32_t time);

	
	//更新
	void Update();

	

	//完了ならtrueを返す
	bool IsFinished() { 
		return isFinish_;
	}





private:
	//コールバック
	//呼び出したい関数std::function
	std::function<void()> f_;


	//残り時間
	int32_t time_;
	//完了フラグ
	bool isFinish_ = false;




};

