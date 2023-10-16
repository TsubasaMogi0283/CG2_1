#pragma once
#include <d3d12.h>
#include <cassert>

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

	//デリート代わり
	void DeleteInstance();

	//コマンドに積む専用のGetter
	ID3D12RootSignature* GetSpriteRootSignature() {
		return psoSprite_.rootSignature_;
	}
	ID3D12PipelineState* GetSpriteGraphicsPipelineState() {
		return psoSprite_.graphicsPipelineState_;
	}

	//コマンドに積む専用のGetter
	ID3D12RootSignature* GetModelRootSignature() {
		return psoModel_.rootSignature_;
	}
	ID3D12PipelineState* GetModelGraphicsPipelineState() {
		return psoModel_.graphicsPipelineState_;
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
	//メンバ変数
	struct PipelineVariable {
		ID3DBlob* signatureBlob_ = nullptr;

		ID3DBlob* errorBlob_ = nullptr;

		ID3D12RootSignature* rootSignature_ = nullptr;

		IDxcBlob* pixelShaderBlob_ = nullptr;

		IDxcBlob* vertexShaderBlob_ = nullptr;

		ID3D12PipelineState* graphicsPipelineState_ = nullptr;
	};

	PipelineVariable psoSprite_;
	PipelineVariable psoModel_ ;
};

