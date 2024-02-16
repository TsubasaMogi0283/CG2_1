#pragma once


#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#include "xaudio2fx.h"
#include <fstream>
#include <x3daudio.h>
#include <mmsystem.h>
#include <cassert>
#include <array>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"xaudio2.lib")

#include <wrl.h>
using Microsoft::WRL::ComPtr;

#include "Audio/AudioStruct.h"


class Audio final {
private:
	//コンストラクタ
	Audio();

	//デストラクタ
	~Audio();
public:
	//インスタンスの取得
	static Audio* GetInstance();

	//コピーコンストラクタ禁止
	Audio(const Audio& obj) = delete;

	//代入演算子を無効にする
	Audio& operator=(const Audio& obj) = delete;

public:

	//初期化
	void Initialize();

	//読み込み
	static uint32_t LoadWave(const char* fileName);

	//音声再生
	void PlayWave(uint32_t audioHandle,bool isLoop);

	//音声停止
	void StopWave(uint32_t audioHandle);

	//音量調節
	void ChangeVolume(uint32_t audioHandle,float volume);

	//ピッチの変更(滑らか版)
	void ChangeFrequency(uint32_t audioHandle,float ratio);
	
	//ピッチの変更
	//シンセとかのように段階的に出来るよ
	void ChangePitch(uint32_t audioHandle, int32_t scale);

	//Pan振り
	void SetPan(uint32_t audioHandle, float_t pan);

	//リバーブ
	void CreateReverb(uint32_t audioHandle);

	//エフェクトの効果を無効にする
	void OffEffect(uint32_t audioHandle);

	//エフェクトの効果を有効にする
	void OnEffect(uint32_t audioHandle);


	//解放
	void Release();
	//デバッグ用
	void RatioCalculationDebug();

private:
	//音声データの開放
	void SoundUnload(uint32_t soundDataHandle);


private:
	//IXAudio2はCOMオブジェクトなのでComPtr管理
	ComPtr<IXAudio2> xAudio2_=nullptr;
	//マスターボイス
	IXAudio2MasteringVoice* masterVoice_=nullptr;
	
	XAUDIO2_BUFFER buf_{};

	//Panに必要な変数
	DWORD dwChannelMask_ = {};
	float outputMatrix_[8] = {};
	float left_ = 0.0f;
	float right_ = 0.0f;

	//エフェクト
	IUnknown* pXAPO_ = nullptr;


	//XAUDIO2_EFFECT_DESCRIPTORにデータを設定
	XAUDIO2_EFFECT_DESCRIPTOR descriptor_;

	//XAUDIO2_EFFECT_CHAINにデータを設定
	XAUDIO2_EFFECT_CHAIN chain_;

	//リバーブエフェクト
	XAUDIO2FX_REVERB_PARAMETERS reverbParameters_;



	//構造体版
	//Texturemanagerとだいたい同じ感じにした
	//音声データの最大数
	static const int SOUND_DATE_MAX_ = 256;
	std::array<AudioInformation, SOUND_DATE_MAX_> audioInformation_{};



private:

	//自分のエンジンではA4は442Hz基準にする
	//もちろん12段階で1オクターブ
	static const int SCALE_AMOUNT_ = 13;
	const float SEMITONE_RATIO_[SCALE_AMOUNT_] = {
		1.00000f, //C4
		1.05946f, //C#4
		1.12246f, //D4
		1.18921f, //D#4
		1.25992f, //E4
		1.33483f, //F4
		1.41421f, //F#4
		1.49831f, //G4
		1.58740f, //G#4
		1.68179f, //A4
		1.78180f, //A#4
		1.88775f, //B4
		2.00000f  //C5
	};

	//低い方
	const float MINUS_SEMITONE_RATION[SCALE_AMOUNT_] = {
		1.00000f,	//C4
		0.94387f,	//B3
		0.89090f,	//A3#
		0.84090f,	//A3
		0.79370f,	//G#3
		0.74915f,	//G3
		0.70711f,	//F#3
		0.66742f,	//F3
		0.62996f,	//E3
		0.59460f,	//D#3
		0.56123f,	//D3
		0.52973f,	//C#3
		0.50000f,	//C3
	};


};