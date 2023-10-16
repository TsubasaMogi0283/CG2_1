#include "Audio.h"

Audio* Audio::instance_ = nullptr;

//コンストラクタ
Audio::Audio() {

}

Audio* Audio::GetInstance() {
	if (instance_ == nullptr) {
		instance_ = new Audio();

	}

	return instance_;
}

//デリート代わりの関数
void Audio::DeleteInstance() {
	delete instance_;
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
SoundData Audio::LoadWave(const char* fileName) {
	
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
	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;

	#pragma endregion

	

}

//音声再生
void Audio::PlayWave(const SoundData& soundData,bool isLoop) {
	HRESULT hr{};
	
	
	//波形フォーマットを基にSourceVoiceの生成
	//IXAudio2SourceVoice* pSourceVoice = nullptr;
	hr = xAudio2_->CreateSourceVoice(&pSourceVoice_, &soundData.wfex);
	assert(SUCCEEDED(hr));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (isLoop == true) {
		//ずっとループさせたいならLoopCountにXAUDIO2_LOOP_INFINITEをいれよう
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//波形データの再生
	hr = pSourceVoice_->SubmitSourceBuffer(&buf);
	hr = pSourceVoice_->Start();


	
}

//音声停止
void Audio::StopWave(const SoundData& soundData) {
	HRESULT hr{};
	hr = pSourceVoice_->Stop();
}


//音声データの開放
void Audio::SoundUnload(SoundData* soundData) {
	//バッファのメモリを解放
	delete[] soundData->pBuffer;
	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};

	//XAudio2解放
	xAudio2_.Reset();
}




//デストラクタ
Audio::~Audio() {

}


