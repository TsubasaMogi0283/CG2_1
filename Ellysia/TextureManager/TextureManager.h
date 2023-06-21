#pragma once
#include <string>
#include "externals/DirectXTex/DirectXTex.h"
#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "Common/DirectX/DirectXInitialization.h"


class TextureManager {
public:

	//コンストラクタ
	TextureManager();

	//初期化
	void Initialize(DirectXInitialization* directXSetup);

	//Textureデータを読む
	//1.TextureデータそのものをCPUで読み込む
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	//2.DirectX12のTextureResourceを作る
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	//3.TextureResourceに1で読んだデータを転送する
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);


	void Release();


	//デストラクタ
	~TextureManager();

private:
	DirectXInitialization* directXSetup_ = nullptr;

	ID3D12Resource* resource_ = nullptr;
	ID3D12Resource* textureResource_ = nullptr;


};