#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

class GameObject;

#include "Shader.h"
#include "Component.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

class Transform : public Component
{
public:
	Transform();

	//Accessors
	Vector3* getPosition();
	Vector3* getRotation();
	Vector3* getScale();

	Matrix4 getModelMatrix();
	
	//Mutators
	void setPosition(Vector3* newPosition);
	void setRotation(Vector3* newRotation);
	void setScale(Vector3* newScale);

	//Component Overrides
	virtual void Update();
	virtual void Render();

	~Transform();

private:
	Vector3* position;
	Vector3* rotation;
	Vector3* scale;

	Matrix4 modelMatrix;
};

#endif