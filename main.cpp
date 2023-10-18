#include "Project/AllGameScene/GameManager/GameManager.h"


#include "Common/Windows/WinApp.h"


////ReportLiveObjects
	//DirectX12より低レベルのDXGIに問い合わせをする
	//リソースリークチェック
struct D3DResourceLeakChecker{
	~D3DResourceLeakChecker() {
		ComPtr<IDXGIDebug1>debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
		{
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			
		}


	
	}
};

//staticはdeleteの後に解放されるよ
static D3DResourceLeakChecker checker;

//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager* gameManager = new GameManager();


	gameManager->Operate();

	delete gameManager;
	
	return 0;
}

