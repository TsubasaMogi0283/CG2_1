#pragma once
#include <cstdint>


//基本2進数で
//プレイヤー陣営
const uint32_t COLLISION_ATTRIBUTE_PLAYER = 0b1;

//敵陣営
//左に1つシフトする
const uint32_t COLLISION_ATTRIBUTE_ENEMY = 0b1 << 1;

