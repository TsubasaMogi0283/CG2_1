#include "Particle3DManager.h"


#include <random>
#include <vector>


#include "DirectXSetup.h"
#include <PipelineManager.h>

#include <ModelManager.h>
#include <SrvManager/SrvManager.h>

Particle3DManager* Particle3DManager::GetInstance(){
    static Particle3DManager instance;
    return &instance;
}

void Particle3DManager::Initialize(uint32_t modelHandle){

    //やることリスト
    //ランダムエンジンの初期化
   //C++でいうsrandみたいなやつ
    std::random_device seedGenerator;
    std::mt19937 randomEngine(seedGenerator());

    //パイプラインの生成
    PipelineManager::GetInstance()->GenerateParticle3DPSO();
     
    
    //頂点データの初期化
    //頂点リソースの生成
    //頂点バッファビュー作成
    //頂点リソースに頂点データを書き込む

    
    vertices_ = ModelManager::GetInstance()->GetModelData(modelHandle).vertices;

    
    vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * vertices_.size());

    //読み込みのところでバッファインデックスを作った方がよさそう
    //vertexResourceがnullらしい
    //リソースの先頭のアドレスから使う
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    //使用するリソースは頂点のサイズ
    vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * vertices_.size());
    //１頂点あたりのサイズ
    vertexBufferView_.StrideInBytes = sizeof(VertexData);


#pragma region 頂点データ
    //頂点バッファにデータを書き込む
    VertexData* vertexData = nullptr;
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
    std::memcpy(vertexData, vertices_.data(), sizeof(VertexData) * vertices_.size());
    vertexResource_->Unmap(0, nullptr);

#pragma endregion

}

void Particle3DManager::CreateParticleGroup(const std::string name, uint32_t textureHandle){
    


    //空のグループを生成
    ParticleGrounp particles;

    //マテリアルデータにテクスチャグループを作成しする
    // 
    //マテリアルデータにテクスチャのSRVインデックスを記録
    particles.textureSrvindex = textureHandle;
    



    //インスタンス用のリソースを生成
    particles.instancingResource = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * MAX_INSTANCE_NUMBER_);
    
    

    //インスタンシング用にSRVを確保してSRVインデックスを記録
    particles.instancingSrvIndex = 3;
    //SRV生成
    SrvManager::GetInstance()->CreateSRVForStructuredBuffer(3, particles.instancingResource.Get(), MAX_INSTANCE_NUMBER_, sizeof(ParticleForGPU));



    //登録
    particleGroup_.at(name) = particles;



}


