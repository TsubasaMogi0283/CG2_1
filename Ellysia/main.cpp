#include "Ellysia.h"

//staticはdeleteの後に解放されるよ
static D3DResourceLeakChecker checker;

//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Ellysia* myEngine = new Ellysia();

	myEngine->Operate();

	delete myEngine;
	return 0;
}

