#ifndef PRIMITIVE_MANAGER_H
#define PRIMITIVE_MANAGER_H

#include "BrickwareCoreDLL.h"

#include <vector>

#include "Matrix4.h"

#include "Settings.h"
#include "Shader.h"

//Used to avoid warnings about exporting std::vectors
class Primitive;
template class BRICKWARE_CORE_API std::vector <Primitive*>;

/*
	This is effectively a singleton class to the Primitive class however we only want these 
	methods to be exposed to the Game class. 		
*/
class BRICKWARE_CORE_API PrimitiveManager
{
	friend class Game;
	friend class Primitive;

private:
	static void Initialize();
	static void ClearPrimitives();
	static void DrawPrimitives();
	static void Destroy();

	static std::vector<Primitive*> Primitives;
	static Shader* shader;

#ifdef GL_SUPPORT
	//Primitive buffers
	static GLuint pointVBO;
	static GLuint pointIBO;

	static GLuint lineVBO;
	static GLuint lineIBO;

	static GLuint lineQuadVBO;
	static GLuint lineQuadIBO;

	static GLuint lineCircleVBO;
	static GLuint lineCircleIBO;

	static GLuint fillQuadVBO;
	static GLuint fillQuadIBO;

	static GLuint fillCircleVBO;
	static GLuint fillCircleIBO;

	static void BufferDataGL();

	static void DrawPrimitiveGL(Primitive* p);

	static void SetColorGL(Vector4 color);
	static void SetPointSizeGL(float pointSize);
	static void SetLineWidthGL(float lineWidth);
#endif

#ifdef D3D_SUPPORT
	static void BufferDataD3D();

	static void DrawPrimitiveD3D(Primitive* p, Matrix4 worldMatrix);

	static void SetColorD3D(Vector4 color);
	static void SetPointSizeD3D(float pointSize);
	static void SetLineWidthD3D(float lineWidth);
#endif
};

#endif