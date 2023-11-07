

//座標返還を行うVS
struct TransformationMatrix {
	//32bitのfloatが4x4個
	float32_t4x4 WVP;
	float32_t4x4 World;
};

//CBuffer


//StructuredBuffer...hlsl番の配列みたいなやつ
//TransformationMatrix gTransformationMatrices[10];
StructuredBuffer<TransformationMatrix> gTransformationMatrices : register(t0);


struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};


VertexShaderOutput main(VertexShaderInput input,uint32_t instancedId:SV_InstanceID) {
	VertexShaderOutput output;
	//mul...組み込み関数
    output.position = mul(input.position, gTransformationMatrices[instancedId].WVP);
	output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gTransformationMatrices[instancedId].World));
	return output;
}