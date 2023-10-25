#include "Audio.h"


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

//読み込み
uint32_t Audio::LoadWave(const char* fileName) {
	audioHandle_++;

	#pragma region １,ファイルオープン
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(fileName, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

	#pragma endregion

	#pragma region ２,wavデータ読み込み

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	//何かここ空白入れないとダメらしい
	//後ろが4だからかな・・
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);


	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));

	}

	//メインのデータチャンク
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	#pragma endregion

	#pragma region ３,Waveファイルを閉じる
	file.close();


	#pragma endregion

	#pragma region 読み込んだ音声データを返す
	
	soundData[audioHandle_].wfex = format.fmt;
	soundData[audioHandle_].pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData[audioHandle_].bufferSize = data.size;

	return audioHandle_;

	#pragma endregion

	

}

//音声再生
void Audio::PlayWave(uint32_t audioHandle,bool isLoop) {
	HRESULT hr{};
	
	
	//波形フォーマットを基にSourceVoiceの生成
	//IXAudio2SourceVoice* pSourceVoice = nullptr;
	hr = xAudio2_->CreateSourceVoice(&pSourceVoice_[audioHandle], &soundData[audioHandle].wfex);
	assert(SUCCEEDED(hr));

	//再生する波形データの設定
	
	buf_.pAudioData = soundData[audioHandle].pBuffer;
	buf_.AudioBytes = soundData[audioHandle].bufferSize;
	buf_.Flags = XAUDIO2_END_OF_STREAM;
	if (isLoop == true) {
		//ずっとループさせたいならLoopCountにXAUDIO2_LOOP_INFINITEをいれよう
		buf_.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	if (isLoop == false) {
		buf_.LoopCount = XAUDIO2_NO_LOOP_REGION;
	}

	//波形データの再生
	hr = pSourceVoice_[audioHandle]->SubmitSourceBuffer(&buf_);
	hr = pSourceVoice_[audioHandle]->Start();


	
}

void Audio::ChangeVolume(uint32_t audiohandle, float volume) {
	HRESULT hr = {};
	hr = pSourceVoice_[audiohandle]->SetVolume(volume);
}

//音声停止
void Audio::StopWave(uint32_t audioHandle) {
	HRESULT hr{};
	hr = pSourceVoice_[audioHandle]->Stop();
}


//音声データの開放
void Audio::SoundUnload(uint32_t soundDataHandle) {
	//バッファのメモリを解放
	delete[] soundData[soundDataHandle].pBuffer;
	soundData[soundDataHandle].pBuffer = 0;
	soundData[soundDataHandle].bufferSize = 0;
	soundData[soundDataHandle].wfex = {};

	
}

//解放
void Audio::Release() {
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		if (pSourceVoice_[i] != nullptr) {
			pSourceVoice_[i]->DestroyVoice();
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
