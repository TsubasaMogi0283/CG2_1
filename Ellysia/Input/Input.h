#pragma once

//ここでDirectXInputのバージョン設定をする
#define DIRECTINPUT_VERSION	0x0800

#include <dinput.h>
#include <Common/Windows/WinApp.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input {
public:

	Input();

	void Initialize(WinApp* winApp);

	void Update();

	//Push状態
	bool PushKey(BYTE keyNumber);



	~Input();
	

private:

	WinApp* winApp_ = nullptr;


	//DirectInputの初期化
	IDirectInput8* directInput_ = nullptr;

	//キーボードデバイスの生成
	IDirectInputDevice8* keyboard_ = nullptr;

	//全キーの入力状態を取得する
	BYTE key_[256] = {};





};

