#include "Ellysia/Common/Windows/WindowsInitialization.h"
#include "Ellysia/Common/DirectX/DirectXInitialization.h"
#include "Polygon/Triangle/Triangle.h"
#include <ImGuiManager/ImGuiManager.h>

#include "Ellysia/Math/Vector/Vector4.h"
#include "Math/Matrix/Matrix/Matrix4x4.h"
#include "ConvertFunction/ConvertColor/ColorConvert.h"
#include "Ellysia/Math/Vector/Transform.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"


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

	WindowsInitialization* winSetup = new WindowsInitialization();
	DirectXInitialization* directXSetup = new DirectXInitialization();
	ImGuiManager* imGuiManager = new ImGuiManager();
	
	
	//三角形の情報
	const int32_t TRIANGLE_AMOUNT_MAX = 1;

	
	

	//初期化
	winSetup->Initialize(L"DirectX",WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT);
	directXSetup->Initialize(winSetup->GetClientWidth(),winSetup->GetClientHeight(),winSetup->GetHwnd());
	imGuiManager->Initialize(winSetup, directXSetup);


	////三角形について
	Triangle* triangle[TRIANGLE_AMOUNT_MAX];
	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		triangle[i] = new Triangle();
		triangle[i]->Initialize(directXSetup);
		triangle[i]->LoadTexture("Resources/uvChecker.png");
	}


	

	
	Vector4 triangleCoodinateLeft[TRIANGLE_AMOUNT_MAX] = {
		//left
		//一段目
		{-0.5f,-0.5f,0.0f,1.0f}
		//{-1.0f,0.5f,0.0f,1.0f },
		//{-0.6f,0.5f,0.0f,1.0f },
		//{-0.2f,0.5f,0.0f,1.0f },
		//{0.2f,0.5f,0.0f,1.0f },
		//{0.6f,0.5f,0.0f,1.0f },
		////二段目
		//{-1.0f,0.0f,0.0f,1.0f },
		//{-0.6f,0.0f,0.0f,1.0f },
		//{-0.2f,0.0f,0.0f,1.0f },
		//{0.2f,0.0f,0.0f,1.0f },
		//{0.6f,0.0f,0.0f,1.0f },
		////三段目
		//{-1.0f,-0.5f,0.0f,1.0f },
		//{-0.6f,-0.5f,0.0f,1.0f },
		//{-0.2f,-0.5f,0.0f,1.0f },
		//{0.2f,-0.5f,0.0f,1.0f },
		//{0.6f,-0.5f,0.0f,1.0f },

	};

	Vector4 triangleCoodinateTop[TRIANGLE_AMOUNT_MAX] = {
		{0.0f,0.5f,0.0f,1.0f}
		//up
		//{-0.8f,1.0f,0.0f,1.0f },
		//{-0.4f,1.0f,0.0f,1.0f },
		//{0.0f,1.0f,0.0f,1.0f },
		//{0.4f,1.0f,0.0f,1.0f },
		//{0.8f,1.0f,0.0f,1.0f },
		//
		////
		//{-0.8f,0.5f,0.0f,1.0f },
		//{-0.4f,0.5f,0.0f,1.0f },
		//{0.0f,0.5f,0.0f,1.0f },
		//{0.4f,0.5f,0.0f,1.0f },
		//{0.8f,0.5f,0.0f,1.0f },
		////
		//{-0.8f,0.0f,0.0f,1.0f },
		//{-0.4f,0.0f,0.0f,1.0f },
		//{0.0f,0.0f,0.0f,1.0f },
		//{0.4f,0.0f,0.0f,1.0f },
		//{0.8f,0.0f,0.0f,1.0f },
	};

	Vector4 triangleCoodinateRight[TRIANGLE_AMOUNT_MAX] = {
		{0.5f,-0.5f,0.0f,1.0f}
		//right
		//{-0.6f,0.5f,0.0f,1.0f },
		//{-0.2f,0.5f,0.0f,1.0f },
		//{0.2f,0.5f,0.0f,1.0f },
		//{0.6f,0.5f,0.0f,1.0f },
		//{1.0f,0.5f,0.0f,1.0f },
		//
		//{-0.6f,0.0f,0.0f,1.0f },
		//{-0.2f,0.0f,0.0f,1.0f },
		//{0.2f,0.0f,0.0f,1.0f },
		//{0.6f,0.0f,0.0f,1.0f },
		//{1.0f,0.0f,0.0f,1.0f },
		//
		//{-0.6f,-0.5f,0.0f,1.0f },
		//{-0.2f,-0.5f,0.0f,1.0f },
		//{0.2f,-0.5f,0.0f,1.0f },
		//{0.6f,-0.5f,0.0f,1.0f },
		//{1.0f,-0.5f,0.0f,1.0f },

	};
	
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate={ 0.0f,0.0f,0.0f };

	Transform transform2 = { scale,rotate,translate };

	Transform transform[TRIANGLE_AMOUNT_MAX]{
		{scale,rotate,translate },
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//{scale,rotate,translate },
		//{scale,rotate,translate },
	};



	//頂いた関数で色を決めていく
	Vector4 color[TRIANGLE_AMOUNT_MAX] = {
		{ ColorAdapter(WHITE)},//WHITE
		//{ 0.0f,1.0f,0.0f,1.0f },//GREEN
		//{ ColorAdapter(BLUE) },//BLUE
		//{ ColorAdapter(WHITE)  },
		//{ ColorAdapter(PURPLE) },
		//
		//{ 0.0f,0.0f,1.0f,1.0f },
		//{ 1.0f,0.0f,0.0f,1.0f },
		//{ ColorAdapter(BLACK) },
		//{ 0.0f,0.0f,1.0f,1.0f },
		//{ 1.0f,0.0f,0.0f,1.0f },
		//
		//{0.6f,0.5f,0.0f,1.0f },
		//{ColorAdapter(BLUE) },
		//{0.2f,0.5f,0.0f,1.0f },
		//{0.6f,0.5f,0.0f,1.0f },
		//{ColorAdapter(WHITE)  },

	};
	

	
	Transform cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	//CPUで動かす用
	Transform transformSprite = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };


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
			//
			for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
				//y軸回転
				transform[i].rotate.y += 0.03f;
			}
			
			
			//カメラ行列
			Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			Matrix4x4 viewMatrix = Inverse(cameraMatrix);

			//遠視投影行列
			Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WINDOW_SIZE_WIDTH) / float(WINDOW_SIZE_HEIGHT), 0.1f, 100.0f);

			ImGui::Begin("TriangleSRT");

			for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
				ImGui::InputFloat3("Scale",&transform[i].scale.x);
				ImGui::SliderFloat3("ScaleSlide", &transform[i].scale.x, 1.0f,5.0f);
	
				ImGui::InputFloat3("Rotate",&transform[i].rotate.x);
				ImGui::SliderFloat3("RotateSlide", &transform[i].rotate.x, 0.0f,12.0f);
	

				ImGui::InputFloat3("Translate",&transform[i].translate.x);
				ImGui::SliderFloat3("TranslateSlide", &transform[i].translate.x, -5.0f,5.0f);
	
			}
			


			ImGui::End();


			imGuiManager->PreDraw();
			#pragma endregion
			

			#pragma region 描画処理

			imGuiManager->Draw(directXSetup);

			for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
				//描画処理
				triangle[i]->Draw(
					triangleCoodinateLeft[i], 
					triangleCoodinateTop[i],
					triangleCoodinateRight[i],
					transform[i],
					viewMatrix,
					projectionMatrix,
					color[i]
				);
			}

			


			imGuiManager->EndFrame(directXSetup);

			#pragma endregion



			//フレームの終わり
			directXSetup->EndFrame();
			


			
		}
		
	}

	
	for (int i = 0; i < TRIANGLE_AMOUNT_MAX; i++) {
		//描画処理
		triangle[i]->Release();
	}



	
	


	//解放処理
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

