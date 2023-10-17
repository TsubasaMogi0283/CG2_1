#pragma once
#include <d3d12.h>

#include <wrl.h>
using Microsoft::WRL::ComPtr;

class ResourceObject {
public:
	ResourceObject(ID3D12Resource* resource)
		:resource_(resource) {}

	~ResourceObject() {
		if (resource_) {
			resource_->Release();
		}
	}


private:
	ComPtr<ID3D12Resource> resource_;

};