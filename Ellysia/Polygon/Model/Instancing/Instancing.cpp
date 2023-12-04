#include "Instancing.h"
#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

#include <Camera/Camera.h>

static uint32_t descriptorSizeSRV_ = 0u;

Instancing::Instancing(){

}

void Instancing::Initialize(){
	//インスタンシング
	instancingResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(TransformationMatrix) * instanceCount_);
	
	descriptorSizeSRV_ =  DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = instanceCount_;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

	instancingSrvHandleCPU_ = DirectXSetup::GetInstance()->GetCPUDescriptorHandle(
		DirectXSetup::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);
	instancingSrvHandleGPU_ = DirectXSetup::GetGPUDescriptorHandle(
		DirectXSetup::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);

	DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(
		instancingResource_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

	
	//SRTの設定
	for (uint32_t index = 0; index < instanceCount_; ++index) {
		transforms[index].scale = { 1.0f,1.0f,1.0f };
		transforms[index].rotate = { 0.0f,0.0f,0.0f };
		transforms[index].translate = { index*0.1f,index*0.1f,index*0.1f };

	}
	


}

void Instancing::SetGraphicsCommand(){
	//インスタンシング
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < instanceCount_; ++index) {
		Matrix4x4 worldMatrix = MakeAffineMatrix(transforms[index].scale, transforms[index].rotate, transforms[index].translate);
		
		//WVP行列を作成
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));

		instancingData_[index].WVP = worldViewProjectionMatrix;
		instancingData_[index].World = worldMatrix;
	}

}

void Instancing::GraphicsCommand(){
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);
}

Instancing::~Instancing(){

}
