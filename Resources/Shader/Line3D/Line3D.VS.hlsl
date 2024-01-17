#include "Line3D.hlsli"


//座標返還を行うVS
struct TransformationMatrix
{
	//32bitのfloatが4x4個
    float4x4 WVP;
    float4x4 World;
};



struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

//カメラ用
struct CamerTransformationMatrix
{
	
    float4x4 View;
    float4x4 Projection;
    float4x4 Orthographic;
};

//CBuffer
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<CamerTransformationMatrix> gCamerTransformationMatrix : register(b1);


VertexShaderOutput main(VertexShaderInput input){
    
    //VP
    float4x4 viewProjection = mul(gCamerTransformationMatrix.View, gCamerTransformationMatrix.Projection);
    float4x4 wvp = mul(gTransformationMatrix.World, viewProjection);
	
    
    VertexShaderOutput output;
	//mul...組み込み関数
    output.position = mul(input.position, wvp);
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix.World));
    return output;
}