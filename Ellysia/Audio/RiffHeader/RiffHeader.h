#pragma once
#include "Audio/ChunkHeader/ChunkHeader.h"

struct RiffHeader {
	//RIFF
	ChunkHeader chunk;
	//WAVE
	char type[4];
};