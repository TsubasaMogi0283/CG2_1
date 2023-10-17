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




class PipelineManager {
private:

	//コンストラクタ
	PipelineManager();

	//デストラクタ
	~PipelineManager();

public:
	//シングルインスタンス
	static PipelineManager* GetInstance();


	//コマンドに積む専用のGetter
	ID3D12RootSignature* GetSpriteRootSignature() {
		return spriteRootSignature_;
	}
	ID3D12PipelineState* GetSpriteGraphicsPipelineState() {
		return spriteGraphicsPipelineState_;
	}

	//コマンドに積む専用のGetter
	ID3D12RootSignature* GetModelRootSignature() {
		return modelRootSignature_;
	}
	ID3D12PipelineState* GetModelGraphicsPipelineState() {
		return modelGraphicsPipelineState_;
	}

	void Release();

public:
	//private関数
	//アロー演算子の時邪魔になってしまうからここにいれよう

	//スプライト用
	void GenerateSpritePSO();

	//モデル用
	void GenerateModelPSO();

private:


	ID3DBlob* spriteSignatureBlob_ = nullptr;
	ID3DBlob* spriteErrorBlob_ = nullptr;
	ID3D12RootSignature* spriteRootSignature_ = nullptr;
	IDxcBlob* spritePixelShaderBlob_ = nullptr;
	IDxcBlob* spriteVertexShaderBlob_ = nullptr;
	ID3D12PipelineState* spriteGraphicsPipelineState_ = nullptr;


	ID3DBlob* modelSignatureBlob_ = nullptr;
	ID3DBlob* modelErrorBlob_ = nullptr;
	ID3D12RootSignature* modelRootSignature_ = nullptr;
	IDxcBlob* modelPixelShaderBlob_ = nullptr;
	IDxcBlob* modelVertexShaderBlob_ = nullptr;
	ID3D12PipelineState* modelGraphicsPipelineState_ = nullptr;


};

