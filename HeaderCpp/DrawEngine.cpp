#include "DrawEngine.h"

void DrawEngine::Initialize(DirectX* directX)
{
	commandList_ = directX->GetcommandList();
	device_ = directX->GetDevice();
	MakeVertexResource();
	MakeVertexBufferView();
}

void DrawEngine::Draw(Vector4 Rightbottom, Vector4 top, Vector4 Leftbottom)
{
	Vector4* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0] = Rightbottom;
	//上
	vertexData[1] = top;
	//右下
	vertexData[2] = Leftbottom;
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList_->DrawInstanced(3, 1, 0, 0);
}

void DrawEngine::Release()
{
	vertexResource->Release();
}

void DrawEngine::MakeVertexResource()
{
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Vector4) * 3;
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//頂点リソースを作る
	hr = device_->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));
}

void DrawEngine::MakeVertexBufferView()
{
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	vertexBufferView.StrideInBytes = sizeof(Vector4);
}
