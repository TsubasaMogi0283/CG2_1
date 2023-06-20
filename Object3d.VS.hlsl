
//座標返還を行うVS
struct TransformationMatrix {
	//32bitのfloatが4x4個
	float32_t4x4 WVP;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix:register(b0);


struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
};

struct VertexShaderInput {
	float32_t4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	//mul...組み込み関数
	output.position = mul(input.position,gTransformationMatrix.WVP);
	return output;
}
