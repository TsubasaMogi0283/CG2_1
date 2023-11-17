#pragma once
#include <d3d12.h>
#include <cassert>

#include <ConvertFunction/CompileShader/CompileShaderManager.h>
#include <ConvertFunction/ConvertLog/LogConvert.h>
#include <Common/DirectX/DirectXSetup.h>

//透明化を解決するためにPSOをクラス化しよう
//後ブレンドの時大変だからswitch文で切り替えできるようにしたら楽かも
//スプライトはスプライトで同じ情報だからインスタンス１つでいいよね
//同様にモデルも

struct PSOInformation {
	ComPtr<ID3DBlob> signatureBlob_ = nullptr;
	ComPtr<ID3DBlob> errorBlob_ = nullptr;
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;
	ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

};


class PipelineManager {
private:

	//コンストラクタ
	PipelineManager();

	//デストラクタ
	~PipelineManager();

public:
	//シングルインスタンス
	static PipelineManager* GetInstance();



#pragma region アクセッサ

	//コマンドに積む専用のGetter
	ComPtr<ID3D12RootSignature> GetSpriteRootSignature() {
		return spritePSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetSpriteGraphicsPipelineState() {
		return spritePSO_.graphicsPipelineState_;
	}

	//コマンドに積む専用のGetter
	ComPtr<ID3D12RootSignature> GetModelRootSignature() {
		return modelPSO_.rootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetModelGraphicsPipelineState() {
		return modelPSO_.graphicsPipelineState_;
	}

	void SetSpriteBlendMode(uint32_t blendmode) {
		selectSpriteBlendMode_ = blendmode;
	}
	void SetModelBlendMode(uint32_t blendmode) {
		selectModelBlendMode_ = blendmode;
	}

#pragma endregion

public:
	//アロー演算子の時邪魔になってしまうからここにいれよう

	//スプライト用
	static void GenerateSpritePSO();

	//モデル用
	static void GenerateModelPSO();

	//3Dパーティクル用
	static void GenerateParticle3DPSO();

private:

	//スプライト用
	PSOInformation spritePSO_ = {};
	//モデル用の変数
	PSOInformation modelPSO_ = {};
	//モデル用の変数
	PSOInformation particle3DPSO_ = {};



	//スプライト用
	int32_t selectSpriteBlendMode_ = 1;

	//モデル用の
	int32_t selectModelBlendMode_ = 1;

};

