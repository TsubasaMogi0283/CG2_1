#include "Enemy/TimeCall/TimeCall.h"

TimeCall::TimedCall(std::function<void()> f, uint32_t time){
	//FireAndReset
	f_ = f;
	//kFireInterval
	time_ = time;

}


void TimedCall::Update() { 
	if (isFinish_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinish_ = true;
		//コールバック関数呼び出し
		f_();
		
	}




}


