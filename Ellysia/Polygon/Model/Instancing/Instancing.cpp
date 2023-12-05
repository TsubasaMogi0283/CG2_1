#include "Instancing.h"
#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

#include <Camera/Camera.h>
#include <random>

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

	
	//C++でいうsrandみたいなやつ
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	//-1.0から1.0を指定
	std::uniform_real_distribution<float> distributeion(-1.0f, 1.0f);

	//SRTの設定
	for (uint32_t index = 0; index < instanceCount_; ++index) {
		particles_[index].transform.scale = {1.0f,1.0f,1.0f};
		particles_[index].transform.rotate = {0.0f,0.0f,0.0f};
		particles_[index].transform.translate = { distributeion(randomEngine),distributeion(randomEngine),distributeion(randomEngine) };

		//速度の設定
		particles_[index].velocity = { distributeion(randomEngine),distributeion(randomEngine),distributeion(randomEngine)};



	}
	


}

void Instancing::SetGraphicsCommand(){
	//インスタンシング
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < instanceCount_; ++index) {

		///particles_[index].transform.scale =transform.scale;
		///particles_[index].transform.rotate =transform.rotate;
		///particles_[index].transform.translate =transform.translate;
		

		//後でSceneなどで変更できるようにしておく
		const float DELTA_TIME = 1.0f / 60.0f;
		particles_[index].transform.translate.x += particles_[index].velocity.x * DELTA_TIME;
		particles_[index].transform.translate.y += particles_[index].velocity.y * DELTA_TIME;
		particles_[index].transform.translate.z += particles_[index].velocity.z * DELTA_TIME;


		Matrix4x4 worldMatrix = MakeAffineMatrix(particles_[index].transform.scale, particles_[index].transform.rotate, particles_[index].transform.translate);
		
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
