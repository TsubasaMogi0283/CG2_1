#include "Audio.h"

//コンストラクタ
Audio::Audio() {

}

//初期化
//これはDirecX初期化の後に入れてね
void Audio::Initialize() {

	//XAudioのエンジンのインスタンスを生成
	HRESULT hr{};
	hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//マスターボイスを生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);

}

//更新
void Audio::Update() {

}



//デストラクタ
Audio::~Audio() {

}


