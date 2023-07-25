#pragma once

#include "DirectX/DirectXSetup.h"




class Triangle {
public:
	Triangle();

	void Initialize(DirectXSetup* directXSetup);

	void GenarateVertexResource();

	void Draw(Vector4 left,Vector4 top,  Vector4 right);


	void Release();
	
	~Triangle();



private:

	DirectXSetup* directXSetup_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	ID3D12Resource* vertexResouce_=nullptr;

	//Resourceにデータを書き込む
	Vector4* vertexData_ = nullptr;
	

	
	
};
