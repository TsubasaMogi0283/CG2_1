#pragma once
#include <Audio/ChunkHeader/ChunkHeader.h>
#include <xaudio2.h>

//FMTチャンク
struct FormatChunk {
	//fmt
	ChunkHeader chunk;
	//波形フォーマット
	WAVEFORMATEX fmt;

};