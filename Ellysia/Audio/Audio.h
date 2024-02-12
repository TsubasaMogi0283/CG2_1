#pragma once


#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>


#include <fstream>

#include<x3daudio.h>
#include <mmsystem.h>
#include<cassert>
#include <array>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"xaudio2.lib")

#include <wrl.h>
using Microsoft::WRL::ComPtr;


#include "Audio/AudioStruct.h"


class Audio {
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
	void ChangePitch(uint32_t audioHandle, uint32_t scale);

	//解放
	void Release();

private:
	//音声データの開放
	void SoundUnload(uint32_t soundDataHandle);


private:

	

	//IXAudio2はCOMオブジェクトなのでComPtr管理
	ComPtr<IXAudio2> xAudio2_=nullptr;
	IXAudio2MasteringVoice* masterVoice_=nullptr;
	

	XAUDIO2_BUFFER buf_{};


	//構造体版
	//Texturemanagerとだいたい同じ感じにした
	//音声データの最大数
	static const int SOUND_DATE_MAX_ = 256;
	std::array<AudioInformation, SOUND_DATE_MAX_> audioInformation_{};

	//もちろん12段階で1オクターブ
	static const int SCALE_AMOUNT_ = 13;
	const float SEMITONE_RATIO_[SCALE_AMOUNT_] = {
		1.00000f, //C
		1.05946f, //C#
		1.12246f, //D
		1.18921f, //D#
		1.25992f, //E
		1.33483f, //F
		1.41421f, //F#
		1.49831f, //G
		1.58740f, //G#
		1.68179f, //A
		1.78180f, //A#
		1.88775f, //B
		2.00000f  //C(High)
	};
};