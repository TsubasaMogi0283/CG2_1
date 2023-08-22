//#include "Object3d.hlsli"
//#include "Resources/Shader/object3d.hlsli"

///PixelShader
//ここからの出力は色で、OutputManagerに送られて
//ブレンド処理も行い画面に色が打ち込まれる
 
 
//テクスチャを張り付けたり、ライティングを行ったり出来る。
//最も重要なShader

//ラスタライザー
//		↓VertexShadeOutput
//ピクセルシェーダー
//		↓PixelShadeOutput
//	 出力結合



//Material...色など三角形の表面の材質をけっていするもの
struct Material {
	float32_t4 color;
	int32_t enableLighting;
};


struct DirectionalLight {
	//ライトの色
	float32_t4 color;
	//ライトの向き
	float32_t3 direction;
	//ライトの輝度
	float intensity;
};

//
////ConstantBuffer<構造体>変数名:register(b0);
//ConstantBuffer<Material>gMaterial:register(b0);
//Texture2D<float32_t4>gTexture:register(b0);
//SamplerState gSample:register : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

//Textureは基本的にそのまま読まずSamplerを介して読む
//処理方法を記述している
//Samplerを介してを使ってTextureを読むことをSamplingという

//Textureの各PixelのことはTexelという
//Excelみたいだね()

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};
struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
	float32_t2 texcoord : TEXCOORD0;
};


 
PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float32_t4 textureColor = gTexture.Sample(gSampler,input.texcoord);
	//
	//
	////Lightingする場合
	//if (gMaterial.enableLighting != 0) {
	//
	//	//このままdotだと[-1,1]になる。
	//	//光が当たらないところは「当たらない」のでもっと暗くなるわけではない。そこでsaturate関数を使う
	//	//saturate関数は値を[0,1]にclampするもの。エフェクターにもSaturationってあるよね。
	//	float cos = saturate(dot(normalize(input.normal),-gDirectionalLight.direction));
	//	output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
	//}
	//else {
	//	//Lightingしない場合
	//	output.color = gMaterial.color * textureColor;
	//}
	//return output;
	//PixelShaderOutput output;
	//float32_t4 textureColor = gTexture.Sample(gSampler,input.texcoord);
	//if (gMaterial.enableLighting !=0) {//Lightingする場合
	//	float cos = saturate(dot(normalize(input.normal),-gDirectionalLight.direction));
	//	output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
	//}
	//else {//Lightingしない場合
	//	
	//}
	output.color = gMaterial.color * textureColor;
	
	return output;
}