#include "Audio.h"



static uint32_t audioIndex;

//コンストラクタ
Audio::Audio() {

}

Audio* Audio::GetInstance() {
	static Audio instance;
	return &instance;
}

//初期化
//これはDirecX初期化の後に入れてね
void Audio::Initialize() {

	//XAudioのエンジンのインスタンスを生成
	HRESULT hr;
	hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//マスターボイスを生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	

}

#pragma region 実際に使う関数

//音声再生
void Audio::PlayWave(uint32_t audioHandle,bool isLoop) {
	HRESULT hr{};
	
	//波形フォーマットを基にSourceVoiceの生成
	//IXAudio2SourceVoice* pSourceVoice = nullptr;
	//hr = xAudio2_->CreateSourceVoice(&pSourceVoice_[audioHandle], &soundData[audioHandle].wfex);
	hr = xAudio2_->CreateSourceVoice(&audioInformation_[audioHandle].pSourceVoice_, &audioInformation_[audioHandle].soundData_.wfex);

	assert(SUCCEEDED(hr));

	//再生する波形データの設定
	
	buf_.pAudioData = audioInformation_[audioHandle].soundData_.pBuffer;
	buf_.AudioBytes = audioInformation_[audioHandle].soundData_.bufferSize;
	buf_.Flags = XAUDIO2_END_OF_STREAM;
	if (isLoop == true) {
		//ずっとループさせたいならLoopCountにXAUDIO2_LOOP_INFINITEをいれよう
		buf_.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	if (isLoop == false) {
		buf_.LoopCount = XAUDIO2_NO_LOOP_REGION;
	}

	//波形データの再生
	hr = audioInformation_[audioHandle].pSourceVoice_->SubmitSourceBuffer(&buf_);
	hr = audioInformation_[audioHandle].pSourceVoice_->Start();


	assert(SUCCEEDED(hr));
}

//音量を変える
void Audio::ChangeVolume(uint32_t audioHandle, float volume) {

	HRESULT hr = {};
	hr = audioInformation_[audioHandle].pSourceVoice_->SetVolume(volume);
	assert(SUCCEEDED(hr));
}

//音声停止
void Audio::StopWave(uint32_t audioHandle) {
	HRESULT hr{};
	hr = audioInformation_[audioHandle].pSourceVoice_->Stop();
	assert(SUCCEEDED(hr));
}

#pragma endregion


//音声データの開放
//後ろにあるReleaseで使っているよ
void Audio::SoundUnload(uint32_t soundDataHandle) {
	//バッファのメモリを解放

	delete[] audioInformation_[soundDataHandle].soundData_.pBuffer;
	audioInformation_[soundDataHandle].soundData_.pBuffer = 0;
	audioInformation_[soundDataHandle].soundData_.bufferSize = 0;
	audioInformation_[soundDataHandle].soundData_.wfex = {};

	
}

//解放
void Audio::Release() {
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		if (audioInformation_[i].pSourceVoice_ != nullptr) {
			audioInformation_[i].pSourceVoice_->DestroyVoice();
		}
	}

	//XAudio2解放
	xAudio2_.Reset();

	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		SoundUnload(i);
	}

}

//デストラクタ
Audio::~Audio() {

}

