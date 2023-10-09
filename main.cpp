#include "Project/AllGameScene/GameManager/GameManager.h"


#include "Common/Windows/WinApp.h"
//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager* gameManager = new GameManager();


	gameManager->Operate();

	delete gameManager;
	
	return 0;
}

