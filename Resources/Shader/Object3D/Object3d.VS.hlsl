#include "Object3d.hlsli"



//座標返還を行うVS
struct TransformationMatrix
{
	//32bitのfloatが4x4個
    float32_t4x4 World;
    float32_t4x4 Normal;
};

//カメラ用
struct CamerTransformationMatrix{
	
    float32_t4x4 View;
    float32_t4x4 Projection;
    float32_t4x4 Orthographic;
};


//CBuffer
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<CamerTransformationMatrix> gCamerTransformationMatrix : register(b1);




VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	
	
	//VP
    float32_t4x4 viewProjection = mul(gCamerTransformationMatrix.View, gCamerTransformationMatrix.Projection);
    float32_t4x4 wvp = mul(gTransformationMatrix.World, viewProjection);
	
	//mul...組み込み関数。Multiplyの略
    output.position = mul(input.position, wvp);
	output.texcoord = input.texcoord;
	//法線の変換にはWorldMatrixの平衡移動は不要。拡縮回転情報が必要
	//左上3x3だけを取り出す
	//法線と言えば正規化をなのでそれを忘れないようにする
	output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.Normal));
	return output;
}