#include "CreateMaterial.h"
#include <Matrix4x4Calculation.h>



void CreateMaterial::Initialize(){
	materialResource_=DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Material)).Get();
	Material* materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = {1.0f,1.0f,1.0f,1.0f};
	materialData_->enableLighting=false;
	materialData_->enablePhongReflection = false;
	materialData_->shiness = 1.0f;

	materialData_->uvTransform = MakeIdentity4x4();

	materialResource_->Unmap(0, nullptr);

}

void CreateMaterial::SetInformation(Vector4 setColor, bool enableLighting,bool enablePhongReflection,float shiness){
	Material* materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = setColor;
	materialData_->enableLighting=enableLighting;
	materialData_->enablePhongReflection = enablePhongReflection;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shiness = shiness;
	materialResource_->Unmap(0, nullptr);
}


void CreateMaterial::GraphicsCommand(){
	

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());


}