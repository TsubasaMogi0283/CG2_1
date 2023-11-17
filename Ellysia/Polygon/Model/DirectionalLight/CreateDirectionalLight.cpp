#include "CreateDirectionalLight.h"



void CreateDirectionalLight::Initialize(){

	directionalLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DirectionalLight)).Get();
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color={ 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 3.0f;







}


void CreateDirectionalLight::SetGraphicsCommand(){
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

}
