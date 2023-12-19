#pragma once
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <array>
#include <memory>
#include <DirectXTex.h>
#include <d3dx12.h>

#include "Matrix4x4.h"
#include "Vector4.h"
#include "Transform.h"
#include "Material.h"
#include "TransformationMatrix.h"

#include "Matrix4x4Calculation.h"
#include "VertexData.h"


#include "Mesh.h"
#include "CreateMaterial.h"
#include "MaterialData.h"
#include "ModelData.h"
#include "DirectionalLight.h"
#include "CreateDirectionalLight.h"
#include "Transformation.h"
#include <WorldTransform.h>




//ModelObjに全部移すつもり
//ここでは頂点データなどを取り入れるだけのクラスにしたい
class Model {
public:

	//コンストラクタ
	Model();

	//初期化
	//Initializeも兼ねているよ
	//通常
	void CreateObj(const std::string& directoryPath,const std::string& fileName);
	//通常
	static Model* Create(const std::string& directoryPath,const std::string& fileName);
	//ブレンドあり
	static Model* Create(const std::string& directoryPath,const std::string& fileName ,int32_t blendModeNumber );

private:
#pragma region モデルの読み込み関係の関数
	//モデルデータの読み込み
	ModelData LoadObjectFile(const std::string& directoryPath, const std::string& fileName);

	//mtlファイルの読み込み
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);

#pragma endregion

public:
	//WorldTransformつき
	void Draw(WorldTransform& worldTransform);

	//デストラクタ
	~Model();




public:
	//アクセッサのまとめ

	//SRT
	//Scale
	void SetScale(Vector3 scale) {
		this->scale_ = scale;
	}
	const Vector3 GetScale() {
		return scale_;
	}
	//Rotate
	void SetRotate(Vector3 rotate) {
		this->rotate_ = rotate;
	}
	const Vector3 GetRotate() {
		return rotate_;
	}
	//Translate
	void SetTranslate(Vector3 translate) {
		this->translate_ = translate;
	}
	const Vector3 GetTranslate() {
		return translate_;
	}



	
	//透明度の変更
	void SetColor(Vector4 color) {
		this->color_ = color;
	}

	void SetTransparency(float transparency) {
		this->color_.w = transparency;
	}


	//アクセッサのまとめ
	void SetBlendMode(int32_t blendmode) {
		blendModeNumber_ = blendmode;
	}

#pragma region Lightingの設定
	void SetLighting(bool enableLighting) {
		this->isEnableLighting_ = enableLighting;
	}
	//方向
	void SetDirection(Vector3 direction) {
		this->lightingDirection_ = direction;
	}

#pragma endregion



	


private:
	//頂点リソースを作る
	//頂点バッファビューを作成する
	//頂点リソースにデータを書き込む

	//頂点データ
	std::unique_ptr<Mesh> mesh_ = nullptr;

	//マテリアル用のリソースを作る
	std::unique_ptr<CreateMaterial> material_ = nullptr;
	

	//Lighting用
	std::unique_ptr<CreateDirectionalLight> directionalLight_ = nullptr;
	//基本はtrueで
	bool isEnableLighting_ = true;

	//方向
	Vector3 lightingDirection_ = {0.0f,-1.0f,0.0f};




	uint32_t textureHandle_ = 0;

	//SRT
	Vector3 scale_ = { 1.0f,1.0f,1.0f };
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };
	Vector3 translate_ = { 0.0f,0.0f,0.0f };



	//色関係のメンバ変数
	Vector4 color_ = {1.0f,1.0f,1.0f,1.0f};


	

	//TextureManagerを参考にする
	static std::list<ModelData> modelInformationList_;


	//デフォルトはα加算
	int32_t blendModeNumber_ ;

};