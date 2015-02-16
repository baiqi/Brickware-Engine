#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "BrickwareCoreDLL.h"

#include "Mesh.h"
#include "Component.h"
#include "Bounds.h"

class BRICKWARE_CORE_API MeshRenderer : public Component
{
public:
	MeshRenderer(Mesh* mesh);

	Bounds getBounds();

	virtual Component* Clone() override;
	virtual void Update();
	virtual void Render();

	~MeshRenderer();

private:
	Mesh* mesh;
	Bounds bounds;

	void calculateBounds();

	void renderGL(Material* material);
	void renderD3D(Material* material);
};

#endif