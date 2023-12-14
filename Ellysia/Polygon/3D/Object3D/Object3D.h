#pragma once
class Object3D{
public:
	
	Object3D();


	static void StaticInitialize();


	static Object3D* Create();

	void Update();


	void Draw();



	~Object3D();



private:
	void Initialize();



private:
};

