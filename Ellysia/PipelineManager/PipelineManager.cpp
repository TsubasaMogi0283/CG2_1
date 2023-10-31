#include "PipelineManager.h"





//コンストラクタ
PipelineManager::PipelineManager() {

}



PipelineManager* PipelineManager::GetInstance() {
	//関数内static変数として宣言する
	static PipelineManager instance;

	return &instance;
 }




//スプライト用
void PipelineManager::GenerateSpritePSO() {
	
	//PSO
	////RootSignatureを作成
	//RootSignature・・ShaderとResourceをどのように間レンズけるかを示したオブジェクトである
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	descriptionRootSignature_.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


	//rootParameter生成。複数設定できるので配列。
	//今回は結果一つだけなので長さ１の配列

	//VSでもCBufferを利用することになったので設定を追加
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	//CBVを使う
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	////PixelShaderで使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号とバインド
	//register...Shader上のResource配置情報
	rootParameters[0].Descriptor.ShaderRegister = 0;


	//CBVを使う
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertwxShaderで使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報
	rootParameters[1].Descriptor.ShaderRegister = 0;
	//ルートパラメータ配列へのポイント
	descriptionRootSignature_.pParameters = rootParameters;
	//配列の長さ
	descriptionRootSignature_.NumParameters = _countof(rootParameters);

	//rootParameterは今後必要あるたびに追加していく

	//DescriptorRangle
	//複数枚のTexture(SRV)を扱う場合1つづつ設定すると効率低下に繋がる
	//利用する範囲を指定して一括で設定を行う機能のこと
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//0から始まる
	descriptorRange[0].BaseShaderRegister = 0;	
	//数は1つ
	descriptorRange[0].NumDescriptors = 1;	
	//SRVを使う
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	
	 //Offsetを自動計算
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; 


	//DescriptorTableを使う
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	
	//PixelShaderを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;				
	//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	//Tableで利用する数
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CBVを使う
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[3].Descriptor.ShaderRegister = 1;
	

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//バイリニアフィルタ
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//0~1の範囲外をリピート
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//比較しない
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//ありったけのMipmapを使う
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	//レジスタ番号0を使う
	staticSamplers[0].ShaderRegister = 0;
	//PixelShaderで使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);


	//シリアライズしてバイナリにする
	HRESULT hr_ = {};
	hr_ = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &PipelineManager::GetInstance()->spriteSignatureBlob_, &PipelineManager::GetInstance()->spriteErrorBlob_);
	if (FAILED(hr_)) {
		Log(reinterpret_cast<char*>(PipelineManager::GetInstance()->spriteErrorBlob_->GetBufferPointer()));
		assert(false);
	}

	//バイナリを元に生成
	hr_ = DirectXSetup::GetInstance()->GetDevice()->CreateRootSignature(0, PipelineManager::GetInstance()->spriteSignatureBlob_->GetBufferPointer(),
		PipelineManager::GetInstance()->spriteSignatureBlob_->GetBufferSize(), IID_PPV_ARGS(&PipelineManager::GetInstance()->spriteRootSignature_));
	assert(SUCCEEDED(hr_));








	////InputLayout
	//InputLayout・・VertexShaderへ渡す頂点データがどのようなものかを指定するオブジェクト
	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;



	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	






	////BlendStateの設定を行う
	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =D3D12_COLOR_WRITE_ENABLE_ALL;

	//α合成
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend=D3D12_BLEND_INV_SRC_ALPHA;
	
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	



	////RasterizerState
	//RasterizerState・・・Rasterizerに対する設定
	//					  三角形の内部をピクセルに分解して、
	//					  PixelShaderを起動することでこの処理への設定を行う

	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;



	//ShaderをCompileする
	PipelineManager::GetInstance()->spriteVertexShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Object2d.VS.hlsl", L"vs_6_0");
	assert(PipelineManager::GetInstance()->spriteVertexShaderBlob_ != nullptr);



	PipelineManager::GetInstance()->spritePixelShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Object2d.PS.hlsl", L"ps_6_0");
	assert(PipelineManager::GetInstance()->spritePixelShaderBlob_ != nullptr);



	

	////PSO生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	
	graphicsPipelineStateDesc.pRootSignature = PipelineManager::GetInstance()->spriteRootSignature_.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { PipelineManager::GetInstance()->spriteVertexShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->spriteVertexShaderBlob_->GetBufferSize() };
	//vertexShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.PS = { PipelineManager::GetInstance()->spritePixelShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->spritePixelShaderBlob_->GetBufferSize() };
	//pixelShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;


	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ(形状)のタイプ三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むのか設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	//ID3D12PipelineState* graphicsPipelineState_ = nullptr;
	hr_ = DirectXSetup::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&PipelineManager::GetInstance()->spriteGraphicsPipelineState_));
	assert(SUCCEEDED(hr_));


	

}



//モデル用
void PipelineManager::GenerateModelPSO() {
	
	//PSO
	////RootSignatureを作成
	//RootSignature・・ShaderとResourceをどのように間レンズけるかを示したオブジェクトである
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	descriptionRootSignature_.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


	//rootParameter生成。複数設定できるので配列。
	//今回は結果一つだけなので長さ１の配列

	//VSでもCBufferを利用することになったので設定を追加
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	//CBVを使う
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	////PixelShaderで使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号とバインド
	//register...Shader上のResource配置情報
	rootParameters[0].Descriptor.ShaderRegister = 0;


	//CBVを使う
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertwxShaderで使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//register...Shader上のResource配置情報
	rootParameters[1].Descriptor.ShaderRegister = 0;
	//ルートパラメータ配列へのポイント
	descriptionRootSignature_.pParameters = rootParameters;
	//配列の長さ
	descriptionRootSignature_.NumParameters = _countof(rootParameters);

	//rootParameterは今後必要あるたびに追加していく

	//DescriptorRangle
	//複数枚のTexture(SRV)を扱う場合1つづつ設定すると効率低下に繋がる
	//利用する範囲を指定して一括で設定を行う機能のこと
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//0から始まる
	descriptorRange[0].BaseShaderRegister = 0;	
	//数は1つ
	descriptorRange[0].NumDescriptors = 1;	
	//SRVを使う
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;	
	 //Offsetを自動計算
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; 


	//DescriptorTableを使う
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	
	//PixelShaderを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;				
	//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	//Tableで利用する数
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CBVを使う
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixelShaderで使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[3].Descriptor.ShaderRegister = 1;
	

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//バイリニアフィルタ
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//0~1の範囲外をリピート
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//比較しない
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//ありったけのMipmapを使う
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	//レジスタ番号0を使う
	staticSamplers[0].ShaderRegister = 0;
	//PixelShaderで使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);


	//シリアライズしてバイナリにする
	HRESULT hr = {};
	hr = D3D12SerializeRootSignature(&descriptionRootSignature_,
		D3D_ROOT_SIGNATURE_VERSION_1, &PipelineManager::GetInstance()->modelSignatureBlob_, &PipelineManager::GetInstance()->modelErrorBlob_);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(PipelineManager::GetInstance()->modelErrorBlob_->GetBufferPointer()));
		assert(false);
	}

	//バイナリを元に生成
	//ID3D12RootSignature* rootSignature_ = nullptr;
	hr = DirectXSetup::GetInstance()->GetDevice()->CreateRootSignature(0,PipelineManager::GetInstance()->modelSignatureBlob_->GetBufferPointer(),
		PipelineManager::GetInstance()->modelSignatureBlob_->GetBufferSize(), IID_PPV_ARGS(&PipelineManager::GetInstance()->modelRootSignature_));
	assert(SUCCEEDED(hr));








	////InputLayout
	//InputLayout・・VertexShaderへ渡す頂点データがどのようなものかを指定するオブジェクト
	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;



	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	






	////BlendStateの設定を行う
	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =D3D12_COLOR_WRITE_ENABLE_ALL;

	//α合成
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend=D3D12_BLEND_INV_SRC_ALPHA;
	
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	



	////RasterizerState
	//RasterizerState・・・Rasterizerに対する設定
	//					  三角形の内部をピクセルに分解して、
	//					  PixelShaderを起動することでこの処理への設定を行う

	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;



	//ShaderをCompileする
	PipelineManager::GetInstance()->modelVertexShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Object3d.VS.hlsl", L"vs_6_0");
	assert(PipelineManager::GetInstance()->modelVertexShaderBlob_ != nullptr);



	PipelineManager::GetInstance()->modelPixelShaderBlob_ = CompileShaderManager::GetInstance()->CompileShader(L"Resources/Shader/Object3d.PS.hlsl", L"ps_6_0");
	assert(PipelineManager::GetInstance()->modelPixelShaderBlob_ != nullptr);



	

	////PSO生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = PipelineManager::GetInstance()->modelRootSignature_.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { PipelineManager::GetInstance()->modelVertexShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->modelVertexShaderBlob_->GetBufferSize() };
	//vertexShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.PS = { PipelineManager::GetInstance()->modelPixelShaderBlob_->GetBufferPointer(),PipelineManager::GetInstance()->modelPixelShaderBlob_->GetBufferSize() };
	//pixelShaderBlob_->GetBufferSize();
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ(形状)のタイプ三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むのか設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	//ID3D12PipelineState* graphicsPipelineState_ = nullptr;
	hr = DirectXSetup::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&PipelineManager::GetInstance()->modelGraphicsPipelineState_));
	assert(SUCCEEDED(hr));


	

}

//解放
void PipelineManager::Release() {

	

}


//デストラクタ
PipelineManager::~PipelineManager() {

}