#include "Input.h"

Input::Input() {

}

void Input::Initialize(WinApp* winApp) {
	this->winApp_ = winApp;

	HRESULT hr;

	//DirectInputオブジェクトの生成
	//これは一つだけで良い
	hr = DirectInput8Create(
		winApp_->GetHInstance(), 
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8, 
		(void**)&directInput_, nullptr);

	assert(SUCCEEDED(hr));


	//キーボードデバイスの生成
	//GUID_Joystick,GUID_SysMouseを指定して、コントローラーとマウスも使えるよ
	hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));

	
	//入力データの形式のセット
	//標準形式
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));


	//排他制御レベルのセット
	//
	//DISCL_FOREGROUND...画面が手前にある場合のみ入力を受け付ける
	//DISCL_NONEXCLUSIVE...デバイスをこのアプリだけで専有しない
	//DISCL_NOWINKEY...Windowsキーを無効にする

	hr = keyboard_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));


}


void Input::Update() {
	//キーボード情報の取得開始
	keyboard_->Acquire();

	//全キーの入力状態を取得する
	BYTE key[256] = {};
	keyboard_->GetDeviceState(sizeof(key), key);



}



Input::~Input() {

}


