#include "Polygon/Model/Transformation/Transformation.h"
#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>
#include <Common/Windows/WinApp.h>
#include <Camera/Camera.h>



void Transformation::Initialize(){
	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	transformationMatrixResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(TransformationMatrix)).Get();
}

void Transformation::SetInformation(Transform transform){
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));


	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
	//遠視投影行列
	Matrix4x4 viewMatrixSphere = MakeIdentity4x4();
	
	Matrix4x4 projectionMatrixSphere = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::GetInstance()->GetClientWidth()), float(WinApp::GetInstance()->GetClientHeight()), 0.0f, 100.0f);
	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrixSphere = Multiply(worldMatrixSphere, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));


	transformationMatrixData_->WVP = worldViewProjectionMatrixSphere;
	transformationMatrixData_->World =MakeIdentity4x4();

}

void Transformation::SetGraphicCommand(){
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
}
