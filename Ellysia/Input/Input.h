#pragma once

//ここでDirectXInputのバージョン設定をする
#define DIRECTINPUT_VERSION	0x0800

#include <dinput.h>
#include <Common/Windows/WinApp.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include <wrl.h>
using namespace Microsoft::WRL;

class Input {
public:

	Input();

	void Initialize(WinApp* winApp);

	void Update();

	//Push状態
	bool PushKey(uint8_t keyNumber);

	//Trigger状態
	bool TriggerKey(uint8_t keyNumber);


	~Input();
	

private:

	WinApp* winApp_ = nullptr;


	//DirectInputの初期化
	ComPtr<IDirectInput8> directInput_ = nullptr;

	//キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

	//全キーの入力状態を取得する
	BYTE preKey_[256] = {};
	BYTE key_[256] = {};





};

