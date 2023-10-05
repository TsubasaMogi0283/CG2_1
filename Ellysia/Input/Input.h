#pragma once

//ここでDirectXInputのバージョン設定をする
#define DIRECTINPUT_VERSION	0x0800

#include <dinput.h>
#include <Common/Windows/WinApp.h>
#include "Input/Mouse/MouseInformation.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include <wrl.h>
using namespace Microsoft::WRL;



class Input {
public:

	Input();

	void Initialize(WinApp* winApp);

	void Update();


#pragma region キーボード
	//Push状態
	bool IsPushKey(uint8_t keyNumber);

	//Trigger状態
	bool IsTriggerKey(uint8_t keyNumber);

	//Releaseいるかな・・・
	//必要になったら追加する

#pragma endregion
	//intがいいのかな
	//Noviceではint32_tだった
	//Push状態
	bool IsPushMouse(int32_t keyNumber);

	//Trigger状態
	bool IsTriggerMouse(int32_t keyNumber);




	~Input();
	

private:

	WinApp* winApp_ = nullptr;


	//DirectInputの初期化
	ComPtr<IDirectInput8> directInput_ = nullptr;

	//キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
	//マウスのデバイスを生成
	ComPtr<IDirectInputDevice8> mouseDevice_ = nullptr;

	//全キーの入力状態を取得する
	BYTE preKey_[256] = {};
	BYTE currentKey_[256] = {};

	//マウスの入力状態を取得
	DIMOUSESTATE currentMouse_ = {};
	DIMOUSESTATE preMouse_ = {};


};

