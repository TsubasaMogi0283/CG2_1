#pragma once

#include "Matrix4x4.h"



struct Camera {
public:


public:

	//基本はアクセッサだけにする
	void SetRotate(Vector3 rotate) {
		this->cameraTransform_.rotate = rotate;
	}

	void SetTranslate(Vector3 translate) {
		this->cameraTransform_.translate = translate;
	}

	//アフィン行列を取得
	Matrix4x4 GetAffineMatrix();
	
	//ビュー行列を取得
	Matrix4x4 GetViewMatrix();

	//射影行列を取得
	Matrix4x4 GetProjectionMatrix_();

private:
	// 定数バッファ用データ構造体
	struct ConstBufferDataViewProjection {
		Matrix4x4 view;       // ワールド → ビュー変換行列
		Matrix4x4 projection; // ビュー → プロジェクション変換行列
		Vector3 cameraPos;    // カメラ座標（ワールド座標）
	};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;

	// ビュー行列
	Matrix4x4 matView;
	// 射影行列
	Matrix4x4 matProjection;
};
