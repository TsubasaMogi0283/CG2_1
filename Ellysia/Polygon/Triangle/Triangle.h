#pragma once

#include "DirectX/DirectXSetup.h"




class Triangle {
public:
	Triangle();

	void Initialize(DirectXSetup* directXSetup);

	void GenarateVertexResource();

	void Draw();

	void SetVertexData0(Vector4 vD0);
	void SetVertexData1(Vector4 vD1);
	void SetVertexData2(Vector4 vD2);



	void Release();
	
	~Triangle();



private:

	DirectXSetup* directXSetup_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	ID3D12Resource* vertexResouce_=nullptr;

	//Resourceにデータを書き込む
	Vector4* vertexData_ = nullptr;
	

	
	
};
