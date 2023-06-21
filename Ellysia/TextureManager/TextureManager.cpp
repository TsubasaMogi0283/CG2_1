#include "TextureManager/TextureManager.h"


//コンストラクタ
TextureManager::TextureManager() {

}


void TextureManager::Initialize(DirectXInitialization* directXSetup) {
	this->directXSetup_ = directXSetup;

	//Textureを読んで転送する
	DirectX::ScratchImage mipImages= LoadTexture("Resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(directXSetup->GetDevice(), metadata);
	UploadTextureData(textureResource, mipImages);

}




//Textureを読み込むためのLoad関数
//1.TextureデータそのものをCPUで読み込む
DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filePath) {
	
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	
	//ミップマップの作成
	//ミップマップ...元画像より小さなテクスチャ群
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(), 
		image.GetImageCount(),
		image.GetMetadata(), 
		DirectX::TEX_FILTER_SRGB
		, 0, mipImages);

	//ミップマップ月のデータを返す
	return mipImages;
}


//2.DirectX12のTextureResourceを作る
ID3D12Resource* TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
	//1.metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	//Textureの幅
	resourceDesc.Width = UINT(metadata.width);
	//Textureの高さ
	resourceDesc.Height = UINT(metadata.height);
	//mipmapの数
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	//奥行き or 配列Textureの配列数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	//TextureのFormat
	resourceDesc.Format = metadata.format;
	//サンプリングカウント
	resourceDesc.SampleDesc.Count = 1;
	//Textureの次元数。普段使っているのは2次元
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	//2.利用するHeapの設定
	//利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	//細かい設定を行う
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	//WriteBackポリシーでCPUアクセス可能
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//プロセッサの近くに配置
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	

	//3.Resourceを生成する
	ID3D12Resource* resource = nullptr;
	HRESULT hr = directXSetup_->GetDevice()->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ,	//初回のResourceState。Textureは基本読むだけ
		nullptr,							//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	return resource;


}







//3.TextureResourceに1で読んだデータを転送する
void TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {
	//Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		//MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Textureに転送
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),			
			nullptr,				//全領域
			img->pixels,			//元データアドレス
			UINT(img->rowPitch),	//1ラインサイズ
			UINT(img->slicePitch));	//1枚サイズ

		assert(SUCCEEDED(hr));
	}
	


}



//デストラクタ
TextureManager::~TextureManager() {

}