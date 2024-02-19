#include "Object3d.hlsli"

//座標返還を行うVS
struct TransformationMatrix {
	//32bitのfloatが4x4個
    float32_t4x4 world;
    float32_t4x4 normal;
    float32_t4x4 worldInverseTranspose;
};

struct Camera{
	//必要なのはこの3つ
	//ビュー行列
    float32_t4x4 viewMatrix_;
	//射影行列
    float32_t4x4 projectionMatrix_;
	//正射影行列
    float32_t4x4 orthographicMatrix_;
};


//CBuffer
ConstantBuffer<TransformationMatrix> gTransformationMatrix:register(b0);
ConstantBuffer<Camera> gCamera : register(b1);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};


VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	
    float32_t4x4 world = gTransformationMatrix.world;
    float32_t4x4 viewProjection = mul(gCamera.viewMatrix_, gCamera.projectionMatrix_);
	
    float32_t4x4 wvp = mul(world, viewProjection);
	
	//mul...組み込み関数
    output.position = mul(input.position, wvp);
	output.texcoord = input.texcoord;
	//法線の変換にはWorldMatrixの平衡移動は不要。拡縮回転情報が必要
	//左上3x3だけを取り出す
	//法線と言えば正規化をなのでそれを忘れないようにする
    output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.worldInverseTranspose));
	
	//CameraWorldPosition
    output.worldPosition = mul(input.position, gTransformationMatrix.world).xyz;
	return output;
}