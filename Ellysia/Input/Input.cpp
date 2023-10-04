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


//Push状態
bool Input::PushKey(BYTE keyNumber) {
	//指定されていたキーを押していればtrueを返す
	if (key_[keyNumber]) {
		return true;
	}
	//押されていなければfalseを返す
	return false;

}



void Input::Update() {
	//キーボード情報の取得開始
	keyboard_->Acquire();

	
	keyboard_->GetDeviceState(sizeof(key_), key_);

	//0から255番まであるよ
	//エンターキーを押していると0x80(128)が代入されるよ
	//押していないとどのキーも0x00(0)が代入されるよ


}



Input::~Input() {

}


