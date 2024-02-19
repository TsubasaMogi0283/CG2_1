#include "Object3d.hlsli"

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



//Material...色など三角形の表面の材質を決定するもの
struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
    //光沢度
	float32_t shininess;
};


struct DirectionalLight {
	//ライトの色
	float32_t4 color;
	//ライトの向き
	float32_t3 direction;
	//ライトの輝度
	float intensity;
};

//カメラの位置を送る
struct Camera{
    float32_t3 worldPosition;
};


//
////ConstantBuffer<構造体>変数名:register(b0);
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Camera> gCamera : register(b2);

//Textureは基本的にそのまま読まずSamplerを介して読む
//処理方法を記述している
//Samplerを介してを使ってTextureを読むことをSamplingという

//Textureの各PixelのことはTexelという
//Excelみたいだね()

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};



 
PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	
    float32_t3 camera = gCamera.worldPosition;
	
	//Materialを拡張する
	float4 transformedUV = mul(float32_t4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    if (textureColor.a <= 0.5f){
        discard;
    }
	
	//Lightingする場合
    if (gMaterial.enableLighting == 1){
	
		//このままdotだと[-1,1]になる。
		//光が当たらないところは「当たらない」のでもっと暗くなるわけではない。そこでsaturate関数を使う
		//saturate関数は値を[0,1]にclampするもの。エフェクターにもSaturationってあるよね。
	

		//Half Lambert
        float NdotL = dot(normalize(input.normal), -normalize(gDirectionalLight.direction));
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

		
		//Cameraへの方向を算出
        float32_t3 toEye = normalize(gCamera.worldPosition-input.worldPosition );
		
		//入射光の反射ベクトルを求める
        float32_t3 reflectLight = reflect(normalize(gDirectionalLight.direction), normalize(input.normal));
		
		//内積
        //float RdotE = dot(reflectLight, toEye);
		//反射強度
        //float specularPow = pow(saturate(RdotE), gMaterial.shininess);
		
		//HalfVector
        float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NDotH), gMaterial.shininess);
		
		//拡散反射
        float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		//鏡面反射
		//1.0f,1.0f,1.0fの所は反射色。
        float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
		
		
		if (textureColor.a == 0){
			discard;
		}
		
        output.color.rgb = diffuse + specular;
        output.color.a = gMaterial.color.a * textureColor.a;

	}
    else{
	//Lightingしない場合
       output.color = gMaterial.color * textureColor;
    }

	
	
	return output;
}