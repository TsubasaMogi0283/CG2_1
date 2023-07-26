#include "Ellysia/Common/DirectX/DirectXSetup.h"
#include "Ellysia/Common/Windows/WindowsApp.h"
#include "Ellysia/Polygon/Triangle/Triangle.h"
#include <Vector/TriangleCoodinate.h>

#include "Ellysia/Ellysia.h"

//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	
	
	
	////コンストラクタ
	
	Ellysia* ellysia = new Ellysia();
	
	
	


	ellysia->Initialize();


	

	

	MSG msg{};

	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			ellysia->WindowMsg(msg);

			
		}
		else {
			//ゲームの処理
			//directXSetup->BeginFlame(winSetup->GetClientWidth(), winSetup->GetClientHeight());
			ellysia->BeginFlame();

			ellysia->Update();

			ellysia->Draw();
			

			//directXSetup->EndFlame();
			ellysia->EndFlame();

		}

		

		
	}


	ellysia->Release();

	//デストラクタ
	//
	ellysia->~Ellysia();


	
	

	return 0;
}


