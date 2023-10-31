#pragma once
#include <xaudio2.h>
#include <fstream>

#include<x3daudio.h>
#include <mmsystem.h>
#include<cassert>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"xaudio2.lib")

#include <wrl.h>
using Microsoft::WRL::ComPtr;


#include "SoundData/SoundData.h"
#include "RiffHeader/RiffHeader.h"
#include "FormatChunk/FormatChunk.h"
#include "ChunkHeader/ChunkHeader.h"



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
	uint32_t LoadWave(const char* fileName);

	//読み込み
	//void LoadWave(const char* fileName);


	//音声再生
	void PlayWave(uint32_t audioHandle,bool isLoop);

	//音声停止
	void StopWave(uint32_t audioHandle);

	
	//音量調節
	void ChangeVolume(uint32_t audiohandle,float volume);


	//音声データの開放
	void SoundUnload(uint32_t soundDataHandle);

	//解放
	void Release();

private:

	//音声データの最大数
	static const int SOUND_DATE_MAX_ = 256;

	//IXAudio2はCOMオブジェクトなのでComPtr管理
	ComPtr<IXAudio2> xAudio2_=nullptr;
	IXAudio2MasteringVoice* masterVoice_=nullptr;

	//波形フォーマットを基にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice_[SOUND_DATE_MAX_] = {nullptr};


	XAUDIO2_BUFFER buf_{};

	SoundData soundData[SOUND_DATE_MAX_] = {};


	uint32_t audioHandle_ = -1;

};