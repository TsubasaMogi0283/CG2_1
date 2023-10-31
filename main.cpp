#include "Project/AllGameScene/GameManager/GameManager.h"

//staticはdeleteの後に解放されるよ
static D3DResourceLeakChecker checker;

//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager* gameManager = new GameManager();


	gameManager->Operate();

	delete gameManager;
	
	return 0;
}

