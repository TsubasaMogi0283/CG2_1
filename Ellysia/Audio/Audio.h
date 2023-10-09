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

	//デリート代わりの関数
	void DeleteInstance();

	//コピーコンストラクタ禁止
	Audio(const Audio& obj) = delete;

	//代入演算子を無効にする
	Audio& operator=(const Audio& obj) = delete;

public:

	

	//初期化
	void Initialize();

	//読み込み
	SoundData LoadWave(const char* fileName);

	//読み込み
	//void LoadWave(const char* fileName);


	//音声再生
	void PlayWave(const SoundData& soundData,bool isLoop);

	//音声停止
	void StopWave(const SoundData& soundData);




	//音声データの開放
	void SoundUnload(SoundData* soundData);




private:
	static Audio* instance_;

	//IXAudio2はCOMオブジェクトなのでComPtr管理
	ComPtr<IXAudio2> xAudio2_=nullptr;
	IXAudio2MasteringVoice* masterVoice_=nullptr;



	bool isUsedAudioIndex = {false};

	//波形フォーマットを基にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice_ = { nullptr };



};