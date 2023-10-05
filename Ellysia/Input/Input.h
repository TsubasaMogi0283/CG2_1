#pragma once

//ここでDirectXInputのバージョン設定をする
#define DIRECTINPUT_VERSION	0x0800

#include <dinput.h>
#include <Xinput.h>
#include <wrl.h>
using namespace Microsoft::WRL;


#include <Common/Windows/WinApp.h>
#include "Input/Mouse/MouseInformation.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")




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

#pragma region マウス

	//intがいいのかな
	//Noviceではint32_tだった
	//Push状態
	bool IsPushMouse(int32_t keyNumber);

	//Trigger状態
	bool IsTriggerMouse(int32_t keyNumber);

#pragma endregion

#pragma region コントローラー

	bool GetJoystickState(XINPUT_STATE& state);


	bool IsPushLeft(XINPUT_STATE& state);
	bool IsPushRight(XINPUT_STATE& state);




#pragma endregion
	

	~Input();

	

private:

	WinApp* winApp_ = nullptr;

	static Input* instance_;

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


	//コントローラー
	XINPUT_STATE state_{};




};

