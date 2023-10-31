#pragma once
#include <cstdint>

//チャンクヘッダ
struct ChunkHeader {
	//チャンク毎のID
	char id[4];
	//チャンクサイズ
	int32_t size;
};