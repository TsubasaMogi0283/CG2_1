#pragma once
//前方宣言
class GameScene;

class IGameSceneAudio {
public:
	virtual ~IGameSceneAudio() {

	};
	virtual void Initialize() = 0;
	virtual void Update(GameScene* gamaManager) = 0;


};