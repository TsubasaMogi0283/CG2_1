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

//float32...32bit=4バイト
//int32_t...4バイト


//Material...色など三角形の表面の材質を決定するもの
struct Material {
	
	float32_t4 color;
	//通常
	int32_t enableLighting;///
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



struct PointLight{
	//ライトの色
    float32_t4 color;
	//ライトの位置
    float32_t3 position;
	//輝度
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
//後でrootparameterで追加してあげる
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);


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
	
	//Materialを拡張する
	float32_t4 transformedUV = mul(float32_t4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	
    if (textureColor.a == 0.0f)
    {
        discard;
    }
	
	//Lightingする場合
    if (gMaterial.enableLighting != 0){
	
		//このままdotだと[-1,1]になる。
		//光が当たらないところは「当たらない」のでもっと暗くなるわけではない。そこでsaturate関数を使う
		//saturate関数は値を[0,1]にclampするもの。エフェクターにもSaturationってあるよね。
		//float cos = saturate(dot(normalize(input.normal),-gDirectionalLight.direction));
	

		//Half Lambert
        float NdotL = dot(normalize(input.normal), -normalize(gDirectionalLight.direction));
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

        if (textureColor.a == 0.0f)
        {
            discard;
        }
		
		
		//カメラへの方向を算出
		//v
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
		
		//DiffuseReflection...拡散反射
		//SpecularReflection...鏡面反射
		
       
        //HalfVectorを求めて計算
		float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NDotH), gMaterial.shininess);
		
		 //入射光の反射ベクトルを求める
		//r
        float32_t3 reflectionLight = reflect(normalize(gDirectionalLight.direction), normalize(input.normal));
		
		//内積をとりsaturate
		//shininessを階乗すると鏡面反射の強度が求まる
		//float RdotE = dot(reflectionLight, toEye);
		//反射強度
		//float specularPow = pow(saturate(RdotE), gMaterial.shininess);
		
		
		
		//DirectionalLight
		////拡散反射
		float32_t3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		
		////鏡面反射
        float32_t3 specularDirectionalLight = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
		
		//入射光の計算
		//物体表面の特定の点に対する入射光を計算する
		float32_t3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
		
        float32_t3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity;
        float32_t3 specularPointLight = gPointLight.color.rgb * gPointLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
		
		
        
		// output.color.rgb = diffuseDirectionalLight + specularDirectionalLight
		
		//拡散反射+鏡面反射
		//Pointも
        output.color.rgb = diffuseDirectionalLight
        + specularDirectionalLight+diffusePointLight + specularPointLight;
		
		
		//アルファは今まで通り
        output.color.a = gMaterial.color.a * textureColor.a;

    }
 //   //PhongReflectionする場合
	//else if (gMaterial.enablePhongReflection!=0){
	//	//カメラへの方向を算出
 //       float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
	//	//入射光反射ベクトルを求める
 //       float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));

	//	//
 //       float RdotE = dot(reflectLight, toEye);
	//	//鏡面反射の強度が求まる
 //       float specularPow = pow(saturate(RdotE), gMaterial.shininess);
		
 //       float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
 //       float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
	//	//全てを一つにする
	//	//拡散反射
 //       float32_t3 diffuse =
	//	gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
	//	//鏡面反射
	//	//float32_t3のところは鏡面反射の色あとで自由に変更できるようにしておく
 //       float32_t3 specular = 
	//	gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);

	//	//拡散反射+鏡面反射
 //       output.color.rgb = diffuse + specular;
	//	//アルファは今までと同じ
 //       output.color.a = gMaterial.color.a * textureColor.a;
 //   }
    else
    {
	//Lightingしない場合
        output.color = gMaterial.color * textureColor;
    }

	
	
	return output;
}