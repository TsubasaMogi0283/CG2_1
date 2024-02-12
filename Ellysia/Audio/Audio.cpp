#include "Audio.h"
#include <imgui.h>



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
//読み込み
uint32_t Audio::LoadWave(const char* fileName) {

	//一度読み込んだものは２度読み込まず返すだけ
	for (int i = 0; i < SOUND_DATE_MAX_; i++) {
		if (Audio::GetInstance()->audioInformation_[i].name_ == fileName) {
			return Audio::GetInstance()->audioInformation_[i].handle_;
		}
	}
	//audioHandle_++;
	audioIndex++;

	//記録
	Audio::GetInstance()->audioInformation_[audioIndex].name_ = fileName;
	Audio::GetInstance()->audioInformation_[audioIndex].handle_ = audioIndex;


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

	Audio::GetInstance()->audioInformation_[audioIndex].soundData_.wfex = format.fmt;
	Audio::GetInstance()->audioInformation_[audioIndex].soundData_.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	Audio::GetInstance()->audioInformation_[audioIndex].soundData_.bufferSize = data.size;



	return audioIndex;

#pragma endregion



}


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

//一応マイナスにも出来るらしい
//移送の反転するために使うらしい。使い道は分からない。
//音量を変える
void Audio::ChangeVolume(uint32_t audioHandle, float volume) {

	HRESULT hr = {};
	hr = audioInformation_[audioHandle].pSourceVoice_->SetVolume(volume);
	assert(SUCCEEDED(hr));
}



//ピッチの変更(滑らか)
void Audio::ChangeFrequency(uint32_t audioHandle,float ratio) {
	HRESULT hr{};

	//これ以上上がらなかった
	if (ratio > 2.0f) {
		ratio = 2.0f;
	}
	//これ以下下がらなかった
	else if (ratio < 0.0f) {
		ratio = 0.0f;
	}

	hr= audioInformation_[audioHandle].pSourceVoice_->SetFrequencyRatio(ratio);
	assert(SUCCEEDED(hr));
}


//ピッチの変更
//シンセとかのように段階的に出来るよ
void Audio::ChangePitch(uint32_t audioHandle, int32_t scale) {

	HRESULT hr{};
	float ratio = 1.0f;
	/*for (uint32_t i = 0; i < SCALE_AMOUNT_; i++) {
		if (scale == i) {
			ratio = SEMITONE_RATIO_[i];
			break;
		}
		
	}*/
	float currentPitchRatio = 1.0f; // 現在のピッチ比率（1.0f は変更なしを表します）
	if (audioInformation_[audioHandle].pSourceVoice_) {
		audioInformation_[audioHandle].pSourceVoice_->GetFrequencyRatio(&currentPitchRatio);
	}
	
	// 半音の変更量に基づいて新しいピッチ比率を計算
	int index = (scale + 12) % 12; // ピアノの音階に従ってインデックスを計算
	float newPitchRatio = currentPitchRatio * SEMITONE_RATIO_[index];

	// 新しいピッチ比率を設定
	hr = audioInformation_[audioHandle].pSourceVoice_->SetFrequencyRatio(newPitchRatio);

	//hr = audioInformation_[audioHandle].pSourceVoice_->SetFrequencyRatio(ratio);
	assert(SUCCEEDED(hr));
}

void Audio::Debug() {
	////const float SEMITONE_RATIO_[SCALE_AMOUNT_] = {
	//	1.00000f, //C
	//	1.05946f, //C#
	//	1.12246f, //D
	//	1.18921f, //D#
	//	1.25992f, //E
	//	1.33483f, //F
	//	1.41421f, //F#
	//	1.49831f, //G
	//	1.58740f, //G#
	//	1.68179f, //A
	//	1.78180f, //A#
	//	1.88775f, //B
	//	2.00000f  //C(High)
	float cC = SEMITONE_RATIO_[1] - SEMITONE_RATIO_[0];
	float Cd = SEMITONE_RATIO_[2] - SEMITONE_RATIO_[1];
	float dD = SEMITONE_RATIO_[3] - SEMITONE_RATIO_[2];
	float De = SEMITONE_RATIO_[4] - SEMITONE_RATIO_[3];



	ImGui::Begin("Audio");
	ImGui::InputFloat("C~C#", &cC);
	ImGui::InputFloat("C#~D", &Cd);
	ImGui::InputFloat("D~D#", &dD);
	ImGui::InputFloat("D#~E", &De);
	ImGui::End();
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

	delete[] Audio::GetInstance()->audioInformation_[soundDataHandle].soundData_.pBuffer;
	Audio::GetInstance()->audioInformation_[soundDataHandle].soundData_.pBuffer = 0;
	Audio::GetInstance()->audioInformation_[soundDataHandle].soundData_.bufferSize = 0;
	Audio::GetInstance()->audioInformation_[soundDataHandle].soundData_.wfex = {};

	
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

