#pragma once
#include "Math/Vector/DirectionalLight.h"
#include "Common/DirectX/DirectXSetup.h"


//#include "Vector4.h" 
//#include "Vector3.h"
//
//struct DirectionalLight {
//	//ライトの色
//	Vector4 color;
//	//ライトの向き
//	Vector3 direction;
//	//輝度
//	float intensity;
//};


//今の所仮置き
//structからこっちに全部移すつもり
//staticがダメだったからこっちにすれば何とかなりそう
class DirectionalLightClass{
	//シングルトンにはしない方がいい
	//個別で設定できるようにしたいから
public:
	void Initialize();

	//Resource作成の関数化
	static ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	void SetGraphicsCommand();




private:

	//Lighting用
	ComPtr<ID3D12Resource> resource_ = nullptr;
	DirectionalLight* data_ = nullptr;

	//関数用
	static D3D12_HEAP_PROPERTIES uploadHeapProperties_;
	static D3D12_RESOURCE_DESC vertexResourceDesc_;
};

