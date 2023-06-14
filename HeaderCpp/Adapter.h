#pragma once
#include "WindowsInitialization.h"
#include "DirectXInitialization.h"
#include "Triangle.h"




class Adapter {
public:
	Adapter();

	void Initialize();


	void WinMSG(MSG& msg);


	void BeginFlame();

	void MakeTriangleVertexResource(const int32_t TRIANGLE_AMOUNT,TriangleProperty TriangleCoodinate[]);

	void Draw(Vector4 top, Vector4 left, Vector4 right, TriangleVertex vertex);

	void EndFlame();

	void Release();

	ID3D12Device* GetDevice() {
		return device_;

	}

	void SetDevice(ID3D12Device* device);
	void SetCommands(Commands commands);


	~Adapter();

private:
	//コンストラクタ
	WindowsInitialization* winSetup = new WindowsInitialization;
	DirectXInitialization* directXSetup = new DirectXInitialization;
	Triangle* triangle = new Triangle();

	const wchar_t* title_=L"DirectX";
	int32_t windowsizeWidth_ = 1280;
	int32_t windowsizeHeight_ = 720;



	//ID3D12GraphicsCommandList* commandList_ = nullptr;
	Commands commands_;
	
	ID3D12Device* device_ = nullptr;

	HRESULT hr_;



};
