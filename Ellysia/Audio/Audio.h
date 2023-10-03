#pragma once
#include <xaudio2.h>
#include <fstream>



#pragma comment(lib,"xaudio2.lib")

#include<x3daudio.h>
#include <mmsystem.h>
#include<cassert>
#pragma comment(lib, "winmm.lib")


using Microsoft::WRL::ComPtr;
#include <wrl.h>




class Audio {
public:

	//コンストラクタ
	Audio();

	//初期化
	void Initialize();

	//更新
	void Update();


	//デストラクタ
	~Audio();







private:
	//IXAudio2はCOMオブジェクトなのでComPtr管理
	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;








};