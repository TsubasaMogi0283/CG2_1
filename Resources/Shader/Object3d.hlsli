//hlsl用のヘッダファイル
//VS/PSの双方でVSOutput構造体を使いたいので
//hlsliに定義を記述して共有できるようにする

struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
	float32_t2 texcoord : TEXCOORD0;
};