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
	ComPtr<ID3D12RootSignature> GetSpriteRootSignature() {
		return spriteRootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetSpriteGraphicsPipelineState() {
		return spriteGraphicsPipelineState_;
	}

	//コマンドに積む専用のGetter
	ComPtr<ID3D12RootSignature> GetModelRootSignature() {
		return modelRootSignature_;
	}
	ComPtr<ID3D12PipelineState> GetModelGraphicsPipelineState() {
		return modelGraphicsPipelineState_;
	}

	void Release();

public:
	//private関数
	//アロー演算子の時邪魔になってしまうからここにいれよう

	//スプライト用
	static void GenerateSpritePSO();

	//モデル用
	static void GenerateModelPSO();

private:


	ComPtr<ID3DBlob> spriteSignatureBlob_ = nullptr;
	ComPtr<ID3DBlob> spriteErrorBlob_ = nullptr;
	ComPtr<ID3D12RootSignature> spriteRootSignature_ = nullptr;
	ComPtr<IDxcBlob> spritePixelShaderBlob_ = nullptr;
	ComPtr<IDxcBlob> spriteVertexShaderBlob_ = nullptr;
	ComPtr<ID3D12PipelineState> spriteGraphicsPipelineState_ = nullptr;


	ComPtr<ID3DBlob> modelSignatureBlob_ = nullptr;
	ComPtr<ID3DBlob> modelErrorBlob_ = nullptr;
	ComPtr<ID3D12RootSignature> modelRootSignature_ = nullptr;
	ComPtr<IDxcBlob> modelPixelShaderBlob_ = nullptr;
	ComPtr<IDxcBlob> modelVertexShaderBlob_ = nullptr;
	ComPtr<ID3D12PipelineState> modelGraphicsPipelineState_ = nullptr;


};

