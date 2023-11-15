#include "Polygon/Model/Transformation/Transformation.h"
#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>
#include <Common/Windows/WinApp.h>
#include <Camera/Camera.h>

//Resource作成の関数化
ComPtr<ID3D12Resource> Transformation::CreateBufferResource(size_t sizeInBytes) {
	//void返り値も忘れずに
	ComPtr<ID3D12Resource> resource = nullptr;
	
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD;

	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc_.Width = sizeInBytes;
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc_.Height = 1;
	vertexResourceDesc_.DepthOrArraySize = 1;
	vertexResourceDesc_.MipLevels = 1;
	vertexResourceDesc_.SampleDesc.Count = 1;

	//バッファの場合はこれにする決まり
	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//実際に頂点リソースを作る
	//ID3D12Resource* vertexResource_ = nullptr;
	
	//次はここで問題
	//hrは調査用
	HRESULT hr;
	hr = DirectXSetup::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}


void Transformation::Initialize(){
	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	transformationMatrixResource_ = CreateBufferResource(sizeof(TransformationMatrix)).Get();
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
