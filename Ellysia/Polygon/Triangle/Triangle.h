#pragma once
#include "Common/DirectX/DirectXInitialization.h"
#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "externals/DirectXTex/DirectXTex.h"

#include "Math/Vector/Vector4.h"
#include "Math/Matrix/Matrix/Matrix4x4.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"
#include "Math/Vector/Transform.h"


#include <string>

class Triangle {
public:
	Triangle();


	void Initialize(DirectXInitialization* directXSetup);

	void GenarateVertexResource();

	////VertexBufferViewを作成
	void GenerateVertexBufferView();

	//Material用のリソースを作る
	void GenerateMaterialResource();

	//TransformationMatrix用のResourceを作る
	void GenerateTransformationMatrixResource();

	void Draw(Vector4 left,Vector4 top,  Vector4 right,Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix ,Vector4 color);


	//Resource作成の関数化
	ID3D12Resource* CreateBufferResource(ID3D12Device* device,size_t sizeInBytes);


	//まとめた方がよさそう
	void LoadTexture(const std::string& filePath);
	
#pragma region テクスチャの読み込み
	//Textureデータを読む
	//1.TextureデータそのものをCPUで読み込む
	DirectX::ScratchImage LoadTextureData(const std::string& filePath);

	//2.DirectX12のTextureResourceを作る
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	//3.TextureResourceに1で読んだデータを転送する
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);


#pragma endregion


	void Release();
	
	~Triangle();





private:

	DirectXInitialization* directXSetup_ = nullptr;

	HRESULT hr_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//Resource
	ID3D12Resource* vertexResouce_;

	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//Resourceにデータを書き込む
	Vector4* vertexData_ = nullptr;
	
	
	//マテリアル用のリソースを作る
	ID3D12Resource* materialResource = nullptr;
	Vector4* materialData_ = nullptr;
	

	//TransformationMatrix用のResource
	ID3D12Resource* wvpResource_ = nullptr;
	Matrix4x4* wvpData_=nullptr;

	
	ID3D12Resource* resource_ = nullptr;
	ID3D12Resource* textureResource_ = nullptr;


};
