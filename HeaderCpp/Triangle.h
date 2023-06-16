#pragma once
#include "Function.h"
#include "DirectXInitialization.h"

class Triangle {
public:
	Triangle();

	void Initialize(DirectXInitialization* directXSetup);

	void GenarateVertexResource();

	void Draw(Vector4 left,Vector4 top,  Vector4 right);


	//Resource作成の関数化
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);


	void Release();
	
	~Triangle();



private:

	DirectXInitialization* directXSetup_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	ID3D12Resource* vertexResouce_;

	//Resourceにデータを書き込む
	Vector4* vertexData_ = nullptr;
	

	HRESULT hr_;
	
};
