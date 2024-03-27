#pragma once
#include <cstdint>
#include <xaudio2.h>
#include <string>

//チャンク...データの塊みたいなもの
	//チャンクヘッダ
struct ChunkHeader {
	//チャンク毎のID
	char id[4];
	//チャンクサイズ
	int32_t size;
};

//FMTチャンク
struct FormatChunk {
	//fmt
	ChunkHeader chunk;
	//波形フォーマット
	WAVEFORMATEX fmt;

};

struct RiffHeader {
	//RIFF
	ChunkHeader chunk;
	//WAVE
	char type[4];
};

//音声データ
struct SoundData {
	//波形フォーマット
	WAVEFORMATEX wfex;

	//バッファの先頭アドレス
	BYTE* pBuffer;

	//バッファのサイズ
	unsigned int bufferSize;
};


struct AudioInformation {

	//読み込んだテクスチャの名前
	const char* name_ = {};

	//サウンドデータ
	SoundData soundData_ = {};

	//波形フォーマットを基にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice_ = nullptr;

	//ハンドル
	uint32_t audioHandle_ = 0;

	//テクスチャハンドル
	uint32_t handle_ = 0;

};