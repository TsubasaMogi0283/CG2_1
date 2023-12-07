#include "Instancing.h"
#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

#include <Camera/Camera.h>

#include <numbers>

static uint32_t descriptorSizeSRV_ = 0u;

Instancing::Instancing(){

}

//生成関数
Particle Instancing::MakeNewParticle(std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> distribute(-1.0f, 1.0f);
	Particle particle;
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	particle.transform.translate = {distribute(randomEngine),distribute(randomEngine),distribute(randomEngine)};
	
	//速度
	particle.velocity = {distribute(randomEngine),distribute(randomEngine),distribute(randomEngine)};

	//Color
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	particle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
	

	//時間
	std::uniform_real_distribution<float> distTime(10.0f, 30.0f);
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;



	return particle;

}

void Instancing::Initialize(std::mt19937& randomEngine,const std::vector<VertexData>& vertices){
	mesh_ = std::make_unique<Mesh>();
	mesh_->Initialize(vertices);

	//インスタンシング
	instancingResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * MAX_INSTANCE_NUMBER_);
	
	descriptorSizeSRV_ =  DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = MAX_INSTANCE_NUMBER_;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	instancingSrvHandleCPU_ = DirectXSetup::GetInstance()->GetCPUDescriptorHandle(
		DirectXSetup::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);
	instancingSrvHandleGPU_ = DirectXSetup::GetGPUDescriptorHandle(
		DirectXSetup::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);

	DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(
		instancingResource_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

	
	


	

	for (std::list<Particle>::iterator particleIterator = particles_.begin();
		particleIterator != particles_.end(); ++particleIterator) {

		//(*particleIterator).particles_.push_back(MakeNewParticle(randomEngine));
		//(*particleIterator).particles_.push_back(MakeNewParticle(randomEngine));
		//(*particleIterator).particles_.push_back(MakeNewParticle(randomEngine));
		
		std::uniform_real_distribution<float> distribute(-1.0f, 1.0f);
		(*particleIterator).transform.scale = {1.0f,1.0f,1.0f};
		(*particleIterator).transform.rotate = {0.0f,0.0f,0.0f};
		(*particleIterator).transform.translate = { distribute(randomEngine),distribute(randomEngine),distribute(randomEngine) };

		

		//速度の設定
		std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
		(*particleIterator).velocity = { distribute(randomEngine),distribute(randomEngine),distribute(randomEngine)};
		(*particleIterator).color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
		//particles_[index].color = Vector4(1.0f,1.0f,1.0f,1.0f);

		//時間
		//Color
		std::uniform_real_distribution<float> distTime(10.0f, 30.0f);
		(*particleIterator).lifeTime = distTime(randomEngine);
		(*particleIterator).currentTime = 0;

	}

	//SRTの設定
	//for (uint32_t index = 0; index < MAX_INSTANCE_NUMBER_; ++index) {
		//particles_[index].transform.scale = {1.0f,1.0f,1.0f};
		//particles_[index].transform.rotate = {0.0f,0.0f,0.0f};
		//particles_[index].transform.translate = { distributeion(randomEngine),distributeion(randomEngine),distributeion(randomEngine) };

		//

		////速度の設定
		//particles_[index].velocity = { distributeion(randomEngine),distributeion(randomEngine),distributeion(randomEngine)};
		//particles_[index].color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
		////particles_[index].color = Vector4(1.0f,1.0f,1.0f,1.0f);

		////時間
		////Color
		//std::uniform_real_distribution<float> distTime(10.0f, 30.0f);
		//particles_[index].lifeTime = distTime(randomEngine);
		//particles_[index].currentTime = 0;


		//particles_[index] = MakeNewParticle(randomEngine);
	//}
	


}

void Instancing::SetGraphicsCommand(bool isBillBordMode){
	
	//インスタンシング
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	
	numInstance_ = 0;
	
	for (std::list<Particle>::iterator particleIterator = particles_.begin();
		particleIterator != particles_.end();) {
		//後でSceneなどで変更できるようにしておく
		
		if ((*particleIterator).lifeTime <=(*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);

			continue;
		}
		
		const float DELTA_TIME = 1.0f / 60.0f;
		(*particleIterator).currentTime += DELTA_TIME;
		(*particleIterator).transform.translate.x += (*particleIterator).velocity.x * DELTA_TIME;
		(*particleIterator).transform.translate.y += (*particleIterator).velocity.y * DELTA_TIME;
		(*particleIterator).transform.translate.z += (*particleIterator).velocity.z * DELTA_TIME;
		

		if (isBillBordMode == true) {
			//Y軸でπ/2回転
			//これからはM_PIじゃなくてstd::numbers::pi_vを使おうね
			Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

			//カメラの回転を適用する
			Matrix4x4 billBoardMatrix = Multiply(backToFrontMatrix, Camera::GetInstance()->GetAffineMatrix());
			//平行成分はいらないよ
			billBoardMatrix.m[3][0] = 0.0f;
			billBoardMatrix.m[3][1] = 0.0f;
			billBoardMatrix.m[3][2] = 0.0f;

			Matrix4x4 scaleMatrix = MakeScaleMatrix((*particleIterator).transform.scale);
			Matrix4x4 translateMatrix = MakeTranslateMatrix( (*particleIterator).transform.translate);

			//パーティクル個別のRotateは関係ないよ
			Matrix4x4 worldMatrix = Multiply(scaleMatrix,Multiply(billBoardMatrix,translateMatrix));
			
			//WVP行列を作成
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));

			instancingData_[numInstance_].WVP = worldViewProjectionMatrix;
			instancingData_[numInstance_].World = worldMatrix;
			instancingData_[numInstance_].color = (*particleIterator).color;

			//アルファはVector4でいうwだね
			float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
			instancingData_[numInstance_].color.w=alpha;


		}
		else if (isBillBordMode == false) {
			//ビルボードやらない版
			Matrix4x4 worldMatrix = MakeAffineMatrix((*particleIterator).transform.scale,(*particleIterator).transform.rotate,(*particleIterator).transform.translate);
			
			//WVP行列を作成
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));

			instancingData_[numInstance_].WVP = worldViewProjectionMatrix;
			instancingData_[numInstance_].World = worldMatrix;
			instancingData_[numInstance_].color = (*particleIterator).color;

			//アルファはVector4でいうwだね
			float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
			instancingData_[numInstance_].color.w=alpha;


		}
		

		
		
		//次のイテレータに進める
		++particleIterator;
		

		++numInstance_;

	}

	//for (uint32_t index = 0; index < MAX_INSTANCE_NUMBER_; ++index) {

	//	
	//	//後でSceneなどで変更できるようにしておく
	//	
	//	if (particles_[index].lifeTime <= particles_[index].currentTime) {
	//		
	//		continue;
	//	}
	//	
	//	const float DELTA_TIME = 1.0f / 60.0f;
	//	particles_[index].currentTime += DELTA_TIME;
	//	particles_[index].transform.translate.x += particles_[index].velocity.x * DELTA_TIME;
	//	particles_[index].transform.translate.y += particles_[index].velocity.y * DELTA_TIME;
	//	particles_[index].transform.translate.z += particles_[index].velocity.z * DELTA_TIME;
	//	

	//	if (isBillBordMode == true) {
	//		//Y軸でπ/2回転
	//		//これからはM_PIじゃなくてstd::numbers::pi_vを使おうね
	//		Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

	//		//カメラの回転を適用する
	//		Matrix4x4 billBoardMatrix = Multiply(backToFrontMatrix, Camera::GetInstance()->GetAffineMatrix());
	//		//平行成分はいらないよ
	//		billBoardMatrix.m[3][0] = 0.0f;
	//		billBoardMatrix.m[3][1] = 0.0f;
	//		billBoardMatrix.m[3][2] = 0.0f;

	//		Matrix4x4 scaleMatrix = MakeScaleMatrix(particles_[index].transform.scale);
	//		Matrix4x4 translateMatrix = MakeTranslateMatrix( particles_[index].transform.translate);

	//		//パーティクル個別のRotateは関係ないよ
	//		Matrix4x4 worldMatrix = Multiply(scaleMatrix,Multiply(billBoardMatrix,translateMatrix));
	//		
	//		//WVP行列を作成
	//		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));

	//		instancingData_[numInstance_].WVP = worldViewProjectionMatrix;
	//		instancingData_[numInstance_].World = worldMatrix;
	//		instancingData_[numInstance_].color = particles_[index].color;

	//		//アルファはVector4でいうwだね
	//		float alpha = 1.0f - (particles_[index].currentTime / particles_[index].lifeTime);
	//		instancingData_[numInstance_].color.w=alpha;


	//	}
	//	else if (isBillBordMode == false) {
	//		//ビルボードやらない版
	//		Matrix4x4 worldMatrix = MakeAffineMatrix(particles_[index].transform.scale,particles_[index].transform.rotate,particles_[index].transform.translate);
	//		
	//		//WVP行列を作成
	//		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));

	//		instancingData_[numInstance_].WVP = worldViewProjectionMatrix;
	//		instancingData_[numInstance_].World = worldMatrix;
	//		instancingData_[numInstance_].color = particles_[index].color;

	//		//アルファはVector4でいうwだね
	//		float alpha = 1.0f - (particles_[index].currentTime / particles_[index].lifeTime);
	//		instancingData_[numInstance_].color.w=alpha;


	//	}
	//	

	//	
	//	

	//	
	//	

	//	++numInstance_;
	//	
	//}
	mesh_->GraphicsCommand();
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);

	DrawCall();
	

}


void Instancing::DrawCall() {
	
	//DrawCall
	mesh_->DrawCall(numInstance_);
	
}

Instancing::~Instancing(){

}
