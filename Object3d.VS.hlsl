//応急処置で消した
// //このまま止まっているのは効率が悪い
//#include "Object3d.hlsli"


//座標返還を行うVS
struct TransformationMatrix {
	//32bitのfloatが4x4個
	float32_t4x4 WVP;
};

//CBuffer
ConstantBuffer<TransformationMatrix> gTransformationMatrix:register(b0);


//struct VertexShaderOutput {
//	float32_t4 position : SV_POSITION;
//};


//struct VertexShaderOutput {
//	float32_t4 position : SV_POSITION;
//	float32_t2 texcoord : TEXCOORD0;
//};

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
	float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	//mul...組み込み関数
	output.position = mul(input.position,gTransformationMatrix.WVP);
	output.texcoord = input.texcoord;
	return output;
}