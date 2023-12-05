#include "Instancing.h"
#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

#include <Camera/Camera.h>

#include "ImGuiManager/ImGuiManager.h"

static uint32_t descriptorSizeSRV_ = 0u;

Instancing::Instancing(){

}

void Instancing::Initialize(std::mt19937& randomEngine,const std::vector<VertexData>& vertices){
	mesh_ = std::make_unique<Mesh>();
	mesh_->Initialize(vertices);

	//インスタンシング
	instancingResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * MAX_INSTANCE_NUMBER_);
	
	descriptorSizeSRV_ =  DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = MAX_INSTANCE_NUMBER_;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	instancingSrvHandleCPU_ = DirectXSetup::GetInstance()->GetCPUDescriptorHandle(
		DirectXSetup::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);
	instancingSrvHandleGPU_ = DirectXSetup::GetGPUDescriptorHandle(
		DirectXSetup::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);

	DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(
		instancingResource_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

	
	
	//-1.0から1.0を指定
	std::uniform_real_distribution<float> distributeion(-1.0f, 1.0f);
	//Color
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

	//SRTの設定
	for (uint32_t index = 0; index < MAX_INSTANCE_NUMBER_; ++index) {
		particles_[index].transform.scale = {1.0f,1.0f,1.0f};
		particles_[index].transform.rotate = {0.0f,0.0f,0.0f};
		particles_[index].transform.translate = { distributeion(randomEngine),distributeion(randomEngine),distributeion(randomEngine) };

		//速度の設定
		particles_[index].velocity = { distributeion(randomEngine),distributeion(randomEngine),distributeion(randomEngine)};
		particles_[index].color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
		//particles_[index].color = Vector4(1.0f,1.0f,1.0f,1.0f);

		//時間
		//Color
		std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
		particles_[index].lifeTime = distTime(randomEngine);
		particles_[index].currentTime = 0;
	}
	


}

void Instancing::SetGraphicsCommand(){
	mesh_->GraphicsCommand();
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);

	//インスタンシング
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	
	numInstance_ = 0;
	
	for (uint32_t index = 0; index < MAX_INSTANCE_NUMBER_; ++index) {

		
		//後でSceneなどで変更できるようにしておく
		
		if (particles_[index].lifeTime <= particles_[index].currentTime) {
			
			continue;
		}
		
		const float DELTA_TIME = 1.0f / 60.0f;
		particles_[index].currentTime += DELTA_TIME;
		particles_[index].transform.translate.x += particles_[index].velocity.x * DELTA_TIME;
		particles_[index].transform.translate.y += particles_[index].velocity.y * DELTA_TIME;
		particles_[index].transform.translate.z += particles_[index].velocity.z * DELTA_TIME;
		

		Matrix4x4 worldMatrix = MakeAffineMatrix(particles_[index].transform.scale, particles_[index].transform.rotate, particles_[index].transform.translate);
		
		//WVP行列を作成
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));

		instancingData_[numInstance_].WVP = worldViewProjectionMatrix;
		instancingData_[numInstance_].World = worldMatrix;
		instancingData_[numInstance_].color = particles_[index].color;


		

		
		

		++numInstance_;
		
	}

	DrawCall();
	

}


void Instancing::DrawCall() {
	
	//DrawCall
	mesh_->DrawCall(numInstance_);
	
}

Instancing::~Instancing(){

}
