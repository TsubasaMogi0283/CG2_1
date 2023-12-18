#include "ModelManager.h"

ModelManager* ModelManager::GetInstance(){
	//関数内static変数として宣言する
	static ModelManager instance;
	return &instance;
}
