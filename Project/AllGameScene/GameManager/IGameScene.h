#pragma once

//前方宣言
class GameManager;

class IGameScene {
public:
	virtual ~IGameScene() {
	
	};
	virtual void Initialize(GameManager* gamaManager)=0;
	virtual void Update(GameManager* gamaManager)=0;
	virtual void Draw(GameManager* gamaManager)=0;


};