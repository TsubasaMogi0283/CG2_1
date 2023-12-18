#include "Model.h"

class ModelManager final{
public:

	//コンストラクタ
	ModelManager() = default;

	//デストラクタ
	~ModelManager() = default;

public:
	//インスタンス
	static ModelManager* GetInstance();

	//コピーコンストラクタ禁止
	ModelManager(const ModelManager& modelManager) = delete;

	//代入演算子を無効にする
	ModelManager& operator=(const ModelManager& modelManager) = delete;



};
