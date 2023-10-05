#include "Common/Windows/WinApp.h"
#include "Common/DirectX/DirectXSetup.h"
#include "Polygon/Triangle/Triangle.h"
#include <ImGuiManager/ImGuiManager.h>

#include "Ellysia/Math/Vector/Vector4.h"
#include "Math/Matrix/Matrix/Matrix4x4.h"
#include "ConvertFunction/ConvertColor/ColorConvert.h"
#include "Ellysia/Math/Vector/Transform.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"
#include <Polygon/Sprite/Sprite.h>
#include <Polygon/Sphere/Sphere.h>
#include <Polygon/Model/Model.h>

#include "Audio/Audio.h"
#include <Input/Input.h>


//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//COMの初期化
	//COM...ComponentObjectModel、Microsoftの提唱する設計技術の１つ
	//		DirectX12も簡略化されたCOM(Nano-COM)という設計で作られている
	
	//COMを使用して開発されたソフトウェア部品をCOMコンポーネントと呼ぶ
	//Textureを読むにあたって、COMコンポーネントの１つを利用する
	CoInitializeEx(0, COINIT_MULTITHREADED);

	
	//ウィンドウのサイズを決める
	const int32_t WINDOW_SIZE_WIDTH = 1280;
	const int32_t WINDOW_SIZE_HEIGHT = 720;


	//コンストラクタ
	//
	//いつかKAMATA ENGINEみたいにGameSceneでまとめたい
	//GameScene* gamescene = new GameScene();

	//Model,Sphere, Sprite,TriangleをEllysiaでまとめた方がよさそう

	WinApp* winSetup = new WinApp();
	DirectXSetup* directXSetup = new DirectXSetup();
	ImGuiManager* imGuiManager = new ImGuiManager();
	Input* input = new Input();
	
	
	

	//初期化
	winSetup->Initialize(L"DirectX",WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT);
	directXSetup->Initialize(winSetup->GetClientWidth(),winSetup->GetClientHeight(),winSetup->GetHwnd());
	imGuiManager->Initialize(winSetup, directXSetup);
	input->Initialize(winSetup);




	Model* plane = new Model();
	plane->Initialize(directXSetup);



	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate={ 0.0f,0.0f,0.0f };

	
	Transform cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	
#pragma region Sprite用の座標
	//左下
	Vector4  leftBottom= { 0.0f,360.0f,0.0f,1.0f};
	//左上
	Vector4 leftTop = { 0.0f,0.0f,0.0f,1.0f};
	//右下
	Vector4 rightBottom = { 640.0f,360.0f,0.0f,1.0f} ;
	//上2
	Vector4 rightTop = { 640.0f,0.0f,0.0f,1.0f};

	Transform transformSprite = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

#pragma endregion

	SphereStruct sphereCoodinate = { {-5.0f,-2.0f,20.0f},1.0f};
	Transform transformSphere = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	int number = 0;
	

	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			//common_->WinMSG(msg);
			winSetup->WindowsMSG(msg);

		}
		else {
			//ゲームの処理

			//フレームの開始
			directXSetup->BeginFrame();
			imGuiManager->BeginFrame();

			#pragma region 更新処理

			imGuiManager->UpDate();

			//入力の更新
			input->Update();




			//カメラ行列
			Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			Matrix4x4 viewMatrix = Inverse(cameraMatrix);

			//遠視投影行列
			Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WINDOW_SIZE_WIDTH) / float(WINDOW_SIZE_HEIGHT), 0.1f, 100.0f);
			

#pragma region Modelの位置情報
			if (input->IsTriggerMouse(LeftButton) == true) {
				ImGui::Begin("Model:");
				
				
				ImGui::InputFloat3("Scale", &transformSphere.scale.x);
				ImGui::SliderFloat3("ScaleSlide", &transformSphere.scale.x, 1.0f,5.0f);

				ImGui::InputFloat3("Rotate", &transformSphere.rotate.x);
				ImGui::SliderFloat3("RotateSlide", &transformSphere.rotate.x, 0.0f,12.0f);

				ImGui::InputFloat3("Translate", &transformSphere.translate.x);
				ImGui::SliderFloat3("TranslateSlide", &transformSphere.translate.x,-10.0f,10.0f);

				ImGui::SliderFloat("Radius", &sphereCoodinate.radius, 0.0f, 5.0f);
				
				number += 1;
				
				ImGui::End();
			}

			
			ImGui::Begin("B");
			ImGui::Text("aa:%d", number);
			ImGui::End();
			

#pragma endregion



			imGuiManager->PreDraw();
			#pragma endregion
			

			#pragma region 描画処理
			imGuiManager->Draw(directXSetup);
			

			plane->Draw(transformSphere, viewMatrix, projectionMatrix);

			imGuiManager->EndFrame(directXSetup);

			#pragma endregion



			//フレームの終わり
			directXSetup->EndFrame();
			


			
		}
		
	}


	//解放処理
	plane->Release();
	
	
	directXSetup->Release();
	winSetup->Close();
	imGuiManager->Release();

	delete directXSetup;
	delete winSetup;
	delete imGuiManager;
	
	//ゲーム終了時にはCOMの終了処理を行っておく
	CoUninitialize();
	return 0;
}

