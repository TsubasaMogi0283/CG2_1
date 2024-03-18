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
	assert(SUCCEEDED(hr));

	//マスターボイスを生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));

	//スピーカ構成を取得
	masterVoice_->GetChannelMask(&dwChannelMask_);

	//一度全部0に初期化
	for (int i = 0; i < 8; i++) {
		outputMatrix_[i] = 0;
	}

#pragma region エフェクトチェーンの作成

	//効果を作成
	//この関数でリバーブ効果が生成される
	//hr = XAudio2CreateReverb(&pXAPO_);
	CreateFX(__uuidof(FXReverb),&pXAPO_);
	assert(SUCCEEDED(hr));


#pragma endregion
}



#pragma region 実際に使う関数
//読み込み
uint32_t Audio::LoadWave(const char* fileName) {

	//16,24,32bitは読み込み出来た
	//64bitも読み込み出来るようにしたいと思ったが一般的に使われないらしい
	//だから32が最大で良いかも。
	//64bitを書き出せるCakewslkすご


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
void Audio::PlayWave(uint32_t audioHandle, bool isLoop) {
	HRESULT hr{};

	//波形フォーマットを基にSourceVoiceの生成
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
void Audio::ChangeFrequency(uint32_t audioHandle, float ratio) {
	HRESULT hr{};

	//これより上がらなかった
	ratio = max(ratio, 2.0f);

	//これより下がらなかった
	ratio = min(ratio, 0.0f);


	hr = audioInformation_[audioHandle].pSourceVoice_->SetFrequencyRatio(ratio);
	assert(SUCCEEDED(hr));
}

//ピッチの変更
//シンセとかのように段階的に出来るよ
void Audio::ChangePitch(uint32_t audioHandle, int32_t scale) {

	HRESULT hr{};
	float ratio = 1.0f;

	//入力された値がプラスだった場合
	if (scale >= 0) {
		//プラスのピッチの方を探す
		for (uint32_t i = 0; i < SCALE_AMOUNT_; i++) {
			if (scale == i) {
				ratio = SEMITONE_RATIO_[i];
				break;
			}

		}
		//12以上は上がらなかった
		if (scale > 12) {
			ratio = SEMITONE_RATIO_[12];
		}

	}
	//入力された値がマイナスだった場合
	else if (scale < 0) {
		//マイナスのピッチの方を探す
		for (int32_t i = 0; i < SCALE_AMOUNT_; i++) {
			if (scale == -i) {
				ratio = MINUS_SEMITONE_RATION[i];
				break;
			}

		}
	}


	hr = audioInformation_[audioHandle].pSourceVoice_->SetFrequencyRatio(ratio);
	assert(SUCCEEDED(hr));
}

//Pan振り
void Audio::SetPan(uint32_t audioHandle, float_t pan) {

	//左右のスピーカー間の目的のパンに基づき送信レベルを計算
	left_ = 0.5f - pan / 2.0f;
	right_ = 0.5f + pan / 2.0f;
	switch (dwChannelMask_)
	{
	case SPEAKER_MONO:
		outputMatrix_[0] = 1.0f;
		break;
	case SPEAKER_STEREO:
	case SPEAKER_2POINT1:
	case SPEAKER_SURROUND:
		outputMatrix_[1] = left_;
		outputMatrix_[2] = right_;

		break;
	case SPEAKER_QUAD:
		outputMatrix_[0] = left_;
		outputMatrix_[1] = right_;
		outputMatrix_[2] = left_;
		outputMatrix_[3] = right_;
		break;
	case SPEAKER_4POINT1:
		outputMatrix_[0] = left_;
		outputMatrix_[1] = right_;
		outputMatrix_[3] = left_;
		outputMatrix_[4] = right_;
		break;
	case SPEAKER_5POINT1:
	case SPEAKER_7POINT1:
	case SPEAKER_5POINT1_SURROUND:
		outputMatrix_[0] = left_;
		outputMatrix_[1] = right_;
		outputMatrix_[4] = left_;
		outputMatrix_[5] = right_;
		break;
	case SPEAKER_7POINT1_SURROUND:
		outputMatrix_[0] = left_;
		outputMatrix_[1] = right_;
		outputMatrix_[4] = left_;
		outputMatrix_[5] = right_;
		outputMatrix_[6] = left_;
		outputMatrix_[7] = right_;
		break;
	}


#pragma region 解説
	//調べても良く分からなかったのでChatGPTに聞いた
	//outputMatrix_[0]: 主にモノラル音声の場合に使用され、すべての音声を単一のスピーカーに送信します。
	//outputMatrix_[1] : ステレオ音声の場合、左側のスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[2] : ステレオ音声の場合、右側のスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[3] : クアッドフォニックスシステムの場合、左前のスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[4] : クアッドフォニックスシステムの場合、右前のスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[5] : クアッドフォニックスシステムの場合、左後ろのスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[6] : クアッドフォニックスシステムの場合、右後ろのスピーカーに対する音声の振幅を指定します。
	//outputMatrix_[7] : 7.1サラウンドシステムなどのように、サラウンドサウンドをサポートするシステムにおいて、
	//	追加のサラウンドスピーカーに対する音声の振幅を指定するためのものです。
	//	通常、これはセンターバックスピーカーに対する音声の振幅を制御するために使用されます。

	//公式嘘ついてる・・
	//0,1だけだと左しかできないし。
#pragma endregion

	XAUDIO2_VOICE_DETAILS voiceDetails;
	audioInformation_[audioHandle].pSourceVoice_->GetVoiceDetails(&voiceDetails);

	XAUDIO2_VOICE_DETAILS masterVoiiceDetails;
	masterVoice_->GetVoiceDetails(&masterVoiiceDetails);

	audioInformation_[audioHandle].pSourceVoice_->SetOutputMatrix(
		NULL, voiceDetails.InputChannels,
		masterVoiiceDetails.InputChannels,
		outputMatrix_);

}

void Audio::CreateReverb(uint32_t audioHandle) {
	HRESULT hr{};


	//データの設定
	//チェーンに複数の効果がある場合は
	//XAUDIO_EFFECT_DESCRIPTOR構造体が必要らしい
	effectDescriptor_.InitialState = true;
	effectDescriptor_.OutputChannels = 1;
	effectDescriptor_.pEffect = pXAPO_;

	////チェーンに複数の効果がある場合Effectメンバーに効果の数が含まれる
	effectChain_.EffectCount = 1;
	effectChain_.pEffectDescriptors = &effectDescriptor_;

	reverbParameters_.ReflectionsDelay = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY;
	reverbParameters_.ReverbDelay = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY;
	reverbParameters_.RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
	reverbParameters_.PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	reverbParameters_.PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	reverbParameters_.PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	reverbParameters_.PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	reverbParameters_.EarlyDiffusion = XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION;
	reverbParameters_.LateDiffusion = XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION;
	reverbParameters_.LowEQGain = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN;
	reverbParameters_.LowEQCutoff = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF;
	reverbParameters_.HighEQGain = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN;
	reverbParameters_.HighEQCutoff = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF;
	reverbParameters_.RoomFilterFreq = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ;
	reverbParameters_.RoomFilterMain = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN;
	reverbParameters_.RoomFilterHF = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF;
	reverbParameters_.ReflectionsGain = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN;
	reverbParameters_.ReverbGain = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN;
	reverbParameters_.DecayTime = XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME;
	reverbParameters_.Density = XAUDIO2FX_REVERB_DEFAULT_DENSITY;
	reverbParameters_.RoomSize = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE;
	reverbParameters_.WetDryMix = XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX;


	//ここで上手くいかない
	hr = audioInformation_[audioHandle].pSourceVoice_->SetEffectChain(&effectChain_);
	assert(SUCCEEDED(hr));

	FXREVERB_PARAMETERS XAPOParameters;
	XAPOParameters.Diffusion = FXREVERB_DEFAULT_DIFFUSION;
	XAPOParameters.RoomSize = FXREVERB_DEFAULT_ROOMSIZE;

	hr = audioInformation_[audioHandle].pSourceVoice_->SetEffectParameters(0, &XAPOParameters, sizeof(FXREVERB_PARAMETERS));
	assert(SUCCEEDED(hr));




}


//エフェクトの効果を無効にする
void Audio::OffEffect(uint32_t audioHandle) {
	HRESULT hr{};
	hr = audioInformation_[audioHandle].pSourceVoice_->DisableEffect(0);
	assert(SUCCEEDED(hr));
}

//エフェクトの効果を有効にする
void Audio::OnEffect(uint32_t audioHandle) {
	HRESULT hr{};
	hr = audioInformation_[audioHandle].pSourceVoice_->EnableEffect(0);
	assert(SUCCEEDED(hr));
}

void Audio::RatioCalculationDebug() {
	const float FREQUENCY_4To5[SCALE_AMOUNT_] = {
		262.815f,	//C4	1.0000f
		278.443f,	//C#4
		295.000f,	//D4
		312.541f,	//D#4
		331.126f,	//E4
		350.816f,	//F4
		371.676f,	//F#4
		393.777f,	//G4
		417.192f,	//G#4
		442.000f,	//A4
		468.283f,	//A#4
		496.128f,	//B4
		525.630f    //C5
	};

	const float FREQUENCY_4To3[SCALE_AMOUNT_] = {
		262.815f,	//C4	1.0000f
		248.064f,	//B3
		234.141f,	//A#3
		221.000f,	//A3
		208.596f,	//G#3
		196.889f,	//G3
		185.838f,	//F#3
		175.408f,	//F3
		165.563f,	//E3
		156.271f,	//D#3
		147.500f,	//D3
		139.221f,	//C#3
		131.407f	//C3
	};

	float c = FREQUENCY_4To5[0] / FREQUENCY_4To5[0];
	float C = FREQUENCY_4To5[1] / FREQUENCY_4To5[0];
	float d = FREQUENCY_4To5[2] / FREQUENCY_4To5[0];
	float D = FREQUENCY_4To5[3] / FREQUENCY_4To5[0];
	float e = FREQUENCY_4To5[4] / FREQUENCY_4To5[0];
	float f = FREQUENCY_4To5[5] / FREQUENCY_4To5[0];
	float F = FREQUENCY_4To5[6] / FREQUENCY_4To5[0];
	float g = FREQUENCY_4To5[7] / FREQUENCY_4To5[0];
	float G = FREQUENCY_4To5[8] / FREQUENCY_4To5[0];
	float a = FREQUENCY_4To5[9] / FREQUENCY_4To5[0];
	float A = FREQUENCY_4To5[10] / FREQUENCY_4To5[0];
	float B = FREQUENCY_4To5[11] / FREQUENCY_4To5[0];
	float CHigh = FREQUENCY_4To5[12] / FREQUENCY_4To5[0];



	float b3 = FREQUENCY_4To3[1] / FREQUENCY_4To3[0];
	float A3 = FREQUENCY_4To3[2] / FREQUENCY_4To3[0];
	float a3 = FREQUENCY_4To3[3] / FREQUENCY_4To3[0];
	float G3 = FREQUENCY_4To3[4] / FREQUENCY_4To3[0];
	float g3 = FREQUENCY_4To3[5] / FREQUENCY_4To3[0];
	float F3 = FREQUENCY_4To3[6] / FREQUENCY_4To3[0];
	float f3 = FREQUENCY_4To3[7] / FREQUENCY_4To3[0];
	float e3 = FREQUENCY_4To3[8] / FREQUENCY_4To3[0];
	float D3 = FREQUENCY_4To3[9] / FREQUENCY_4To3[0];
	float d3 = FREQUENCY_4To3[10] / FREQUENCY_4To3[0];
	float C3 = FREQUENCY_4To3[11] / FREQUENCY_4To3[0];
	float c3 = FREQUENCY_4To3[12] / FREQUENCY_4To3[0];



	ImGui::Begin("PlusScale");
	ImGui::InputFloat("C4", &c, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("C#4", &C, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("D4", &d, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("D#4", &D, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("E4", &e, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("F4", &f, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("F#4", &F, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("G4", &g, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("G#4", &G, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("A4", &a, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("A#4", &A, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("B4", &B, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("C5", &CHigh, 0.0f, 0.0f, "%.5f");

	ImGui::End();



	ImGui::Begin("MinusScale");
	ImGui::InputFloat("B3", &b3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("A#3", &A3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("A", &a3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("G#3", &G3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("G3", &g3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("F#3", &F3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("F3", &f3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("E3", &e3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("D#3", &D3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("D3", &d3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("C#3", &C3, 0.0f, 0.0f, "%.5f");
	ImGui::InputFloat("C3", &c3, 0.0f, 0.0f, "%.5f");

	ImGui::End();

}

void Audio::SetFilter(uint32_t audioHandle){


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

	//pXAPO_->Release();
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

