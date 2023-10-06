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

	
	//ここでタイトルバーの名前を決めてね
	const wchar_t* titleBarName = L"Ellysia";

	//ウィンドウのサイズを決める
	const int32_t WINDOW_SIZE_WIDTH = 1280;
	const int32_t WINDOW_SIZE_HEIGHT = 720;


	//コンストラクタ
	//WinとDirectXは一つだけで良いよ
	//何かImGuiもInputもひとつだけで良い気がしてきたのでこっちもやる
	//シングルで
	//main.cpp側ではwinApp=GetInstanceでやってるけど他のクラスでは直接GetInstanceってやった方が楽かも

	WinApp* winApp = WinApp::GetInstance();
	DirectXSetup* directXSetup = new DirectXSetup();

	ImGuiManager* imGuiManager = new ImGuiManager();
	Input* input = new Input();;
	

	//初期化

	WinApp::GetInstance()->Initialize(titleBarName,WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT);
	directXSetup->Initialize(WinApp::GetInstance()->GetClientWidth(),WinApp::GetInstance()->GetClientHeight(),WinApp::GetInstance()->GetHwnd());
	imGuiManager->Initialize(WinApp::GetInstance(), directXSetup);
	input->Initialize(WinApp::GetInstance());




	Model* plane = new Model();
	//plane->LoadDirectX(directXSetup);
	plane->LoadDirectX(directXSetup);

	plane->Initialize("Resources/05_02", "axis.obj");
	plane->LoadTexture("Resources/uvChecker.png");

	
	Model* plane2 = new Model();
	plane2->LoadDirectX(directXSetup);
	plane2->Initialize("Resources/05_02", "plane.obj");
	plane2->LoadTexture("Resources/uvChecker.png");

	

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate={ 0.0f,0.0f,0.0f };

	
	Transform cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-9.8f} };
	
	SphereStruct sphereCoodinate = { {-5.0f,-2.0f,20.0f},1.0f};
	Transform transformSphere = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform transformSphere2 = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,2.0f} };

	Vector4 transparency = { 1.0f,1.0f,1.0f,1.0f };

	

	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			//common_->WinMSG(msg);
			winApp->WindowsMSG(msg);

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

			XINPUT_STATE joyState{};
			plane2->SetColor(transparency);


			ImGui::Begin("Model:");
			
			ImGui::InputFloat4("Transparency", &transparency.x);
			ImGui::SliderFloat4("TransparencySlide", &transparency.x,0.0f,1.0f);

			
			ImGui::InputFloat3("Scale", &transformSphere.scale.x);
			ImGui::SliderFloat3("ScaleSlide", &transformSphere.scale.x, 1.0f, 5.0f);
			
			ImGui::InputFloat3("Rotate", &transformSphere.rotate.x);
			ImGui::SliderFloat3("RotateSlide", &transformSphere.rotate.x, 0.0f, 12.0f);
			
			ImGui::InputFloat3("Translate", &transformSphere.translate.x);
			ImGui::SliderFloat3("TranslateSlide", &transformSphere.translate.x, -10.0f, 10.0f);
			
			
			ImGui::End();
			

			

#pragma endregion



			imGuiManager->PreDraw();
			#pragma endregion
			

			#pragma region 描画処理
			imGuiManager->Draw(directXSetup);
			

			plane->Draw(transformSphere, viewMatrix, projectionMatrix);
			plane2->Draw(transformSphere2, viewMatrix, projectionMatrix);

			imGuiManager->EndFrame(directXSetup);

			#pragma endregion



			//フレームの終わり
			directXSetup->EndFrame();
			


			
		}
		
	}


	//解放処理
	plane->Release();
	plane2->Release();
	
	directXSetup->Release();
	winApp->Close();
	imGuiManager->Release();

	delete directXSetup;

	delete imGuiManager;

	//ゲーム終了時にはCOMの終了処理を行っておく
	CoUninitialize();
	return 0;
}

