#include "CreateMaterial.h"
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>
#include "ImGuiManager/ImGuiManager.h"

void CreateMaterial::Initialize(){
	materialResource_=DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Material)).Get();

}

void CreateMaterial::SetInformation(Vector4 setColor, bool enableLighting){
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = setColor;
	materialData_->enableLighting=enableLighting;

	materialData_->uvTransform = MakeIdentity4x4();

}


void CreateMaterial::SetGraphicsCommand(){
	

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());


}


