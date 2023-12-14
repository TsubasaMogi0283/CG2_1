#include "Object3D.h"

Object3D::Object3D(){

}

void Object3D::Initialize(){

}


void Object3D::StaticInitialize(){
	Initialize();
}

Object3D* Object3D::Create(){
	Object3D* object3D = new Object3D();

	return nullptr;
}

void Object3D::Update(){

}

void Object3D::Draw(){

}

Object3D::~Object3D(){

}

