////VertexShader
struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
};

struct VertexShaderInput {
	float32_t4 position : POSITION0;
};

//SV_POSITION,POSITION0はC++にはない(Semantics)
//Semantics・・その変数の意味を記したもので、GPUやCPUで利用することになる
//SV・・・SystemValueの略GPU(DirectX12)が解釈する変数となる

//SV_POSITIONはVertexShaderの出力の場合、同次クリップ空間上の座標を表す

//頂点入力
//	↓VertexShadeInput
//頂点シェーダー
//　↓VertexShadeOutput
//ラスタライザ



VertexShaderOutput main(VertexShaderInput input){
	VertexShaderOutput output;
	output.position = input.position;


	return output;
}