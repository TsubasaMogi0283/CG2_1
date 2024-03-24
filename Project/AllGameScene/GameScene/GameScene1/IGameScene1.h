#pragma once

//前方宣言
class GameScene1;

class IGameScene1 {
public:
	virtual ~IGameScene1() {

	};
	virtual void Initialize() = 0;
	virtual void Update(GameScene1* gameScene1) = 0;
	virtual void Draw() = 0;




protected:


};