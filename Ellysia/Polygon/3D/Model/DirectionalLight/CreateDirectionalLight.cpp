#include "CreateDirectionalLight.h"


void CreateDirectionalLight::Initialize(){
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	direction_ = { 0.0f,-1.0f,0.0f };
	intensity_ = 1.0f;

	resource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DirectionalLight)).Get();
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	data_->color=color_;
	data_->direction = direction_;
	data_->intensity = intensity_;
	resource_->Unmap(0,nullptr);






}


void CreateDirectionalLight::SetGraphicsCommand(){

	//いつでも変更できるようにするにはUnMapも用意してね
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	data_->color=color_;
	data_->direction = direction_;
	data_->intensity = intensity_;
	resource_->Unmap(0,nullptr);

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, resource_->GetGPUVirtualAddress());

}