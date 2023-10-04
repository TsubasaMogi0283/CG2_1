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
public:

	//コンストラクタ
	Audio();

	//初期化
	void Initialize();

	//読み込み
	SoundData SoundLoadWave(const char* fileName);

	//音声再生
	void PlayWave(const SoundData& soundData);

	//更新
	void Update();


	//音声データの開放
	void SoundUnload(SoundData* soundData);


	//解放
	void Release();

	//デストラクタ
	~Audio();







private:
	//IXAudio2はCOMオブジェクトなのでComPtr管理
	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;








};