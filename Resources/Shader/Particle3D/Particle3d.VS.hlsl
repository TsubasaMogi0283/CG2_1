#include "Particle3d.hlsli"



struct ParticleForGPU{
   // float32_t4x4 WVP;
    float32_t4x4 World;
    float32_t4 color;
};

//カメラ用
struct CamerTransformationMatrix
{
	
    float32_t4x4 View;
    float32_t4x4 Projection;
    float32_t4x4 Orthographic;
};


//CBuffer
//StructuredBuffer...簡単に言えば配列みたいなやつ
//StructuredBuffer<TransformationMatrix>gTransformationMatrices:register(t0);
StructuredBuffer<ParticleForGPU> gParticle : register(t0);
ConstantBuffer<CamerTransformationMatrix> gCamerTransformationMatrix : register(b1);


struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};


VertexShaderOutput main(VertexShaderInput input,uint32_t instanceId:SV_InstanceID)
{
    VertexShaderOutput output;
    
    //VP
    float32_t4x4 viewProjection = mul(gCamerTransformationMatrix.View, gCamerTransformationMatrix.Projection);
    float32_t4x4 wvp = mul(gParticle[instanceId].World, viewProjection);
    
	//mul...組み込み関数
    output.position = mul(input.position, wvp);
    //output.position = mul(input.position, gParticle[instanceId].WVP);
    output.texcoord = input.texcoord;
	//法線の変換にはWorldMatrixの平衡移動は不要。拡縮回転情報が必要
	//左上3x3だけを取り出す
	//法線と言えば正規化をなのでそれを忘れないようにする
	output.normal = normalize(mul(input.normal, (float32_t3x3) gParticle[instanceId].World));
    output.color = gParticle[instanceId].color;
    
    return output;
}