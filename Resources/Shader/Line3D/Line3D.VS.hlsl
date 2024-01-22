#include "Line3D.hlsli"


////座標返還を行うVS
//struct TransformationMatrix
//{
//	//32bitのfloatが4x4個
//    float4x4 WVP;
//    float4x4 World;
//};

struct LineTransformMatrix
{
	//始点
    float4x4 WVP;
    float4x4 World;

	//終点
    //float4x4 endWVP;
    //float4x4 endWorld;
    

};

struct VertexShaderInput
{
    float4 position : SV_POSITION;
    
    //float4 endPosition : SV_POSITION;
    //float3 endNormal : NORMAL0;
};

//カメラ用
struct CamerTransformationMatrix
{
	
    float4x4 View;
    float4x4 Projection;
    float4x4 Orthographic;
};

//CBuffer
ConstantBuffer<LineTransformMatrix> gTransformationMatrix : register(b1);
ConstantBuffer<CamerTransformationMatrix> gCamerTransformationMatrix : register(b2);


VertexShaderOutput main(VertexShaderInput input){
    
    //VP
    float4x4 viewProjection = mul(gCamerTransformationMatrix.View, gCamerTransformationMatrix.Projection);
    
    //World
    float4x4 startWorld = gTransformationMatrix.World;
    //float4x4 endWorld = gTransformationMatrix.endWorld;
    
    
    //WVP
    float4x4 startWVP = mul(startWorld, viewProjection);
    //float4x4 endWVP = mul(endWorld, viewProjection);
	
    
    VertexShaderOutput output;
	//mul...組み込み関数
    output.position = mul(input.position, startWVP);
    
    //output.endPosition = mul(input.endPosition, endWVP);
    //output.endNormal = normalize(mul(input.endNormal, (float3x3) gTransformationMatrix.endWVP));
    
    return output;
}