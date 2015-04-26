#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore\Softbody.hpp"

using namespace Brickware;
using namespace Core;
using namespace Math;
using namespace Graphics;

/*
	Private Spring Implementation
*/
Spring::Spring(Node* node1, Node* node2, float stiffness)
{
	this->node1 = node1;
	this->node2 = node2;

	restingDistance = (node1->position - node2->position).getMagnitude();

	float invMass1 = 1 / node1->mass;
	float invMass2 = 1 / node2->mass;

	float invMassSum = invMass1 + invMass2;

	node1StiffnessScalar = stiffness;
	node2StiffnessScalar = stiffness;
}

void Spring::updateForces()
{
	Vector3 delta = node1->position - node2->position;
	float mag = delta.getMagnitude();
	float distance = (restingDistance - mag) / mag;

	if (restingDistance != 0 && mag != 0 && distance != 0)
	{
		Vector3 force1 = Vector3::Normalize(delta) * (distance * node1StiffnessScalar);
		Vector3 force2 = Vector3::Normalize(delta) * (-distance * node2StiffnessScalar);

		node1->addForce(force1);
		node2->addForce(force2);
	}
}

Spring::~Spring(){}

/*
	Private Node Implementation
*/

Node::Node(Vector3 position)
{
	this->position = position;
	this->restPosition = position;

	frameForce = Vector3();
	acceleration = Vector3();
	velocity = Vector3();

	//Default values
	restVelocity = 0.1f;

	mass = 1.0f;

	pinned = false;
}

void Node::setMass(float mass){ this->mass = mass; }

void Node::addForce(Vector3 force)
{
	frameForce += force; 
}
void Node::addNeighbor(Node* node)
{ 
	if (node == this)
		return;

	bool alreadyAdded = false;

	for (unsigned int i = 0; i < neighbors.size(); i++)
	{
		if (node->position == neighbors[i]->position)
		{
			alreadyAdded = true;
			break;
		}
	}

	if (!alreadyAdded)
		neighbors.push_back(node); 
}

void Node::updateForces()
{
	if (pinned)
		return;

	//frameForce *= GameTime::GetDeltaTime();

	acceleration[1] += (-.0981f*.5f / mass);
	acceleration += (frameForce / mass);
	velocity += acceleration * GameTime::GetDeltaTime();

	position += velocity;

	//Apply basic friction to acceleration
	acceleration *= .90f;

	if (velocity.getMagnitude() <= restVelocity)
		velocity = Vector3();

	frameForce = Vector3();
}

/*
	Softbody Implementation
*/

Softbody::Softbody()
{
	this->mass = 1.0f;
	this->stiffness = .92f;
}

Softbody::Softbody(float nodeMass, float stiffness)
{
	this->mass = nodeMass;
	this->stiffness = stiffness;
}

void Softbody::Start()
{
	//Register into the physics system
	PhysicsManager::AddSoftbody(this);
	
	mesh = getGameObject()->getComponent<MeshRenderer>()->getMesh();

	//Look through mesh data and determine which vertices are connected to each other
	vector<Vector3> vertices = mesh->getVerticies();
	vector<Vector3> indices = mesh->getIndices();

	int vertCounter = 0;
	for (unsigned int i = 0; i < indices.size() - 2; i++)
	{
		int vert1Index = (int)indices[i][0];
		int vert2Index = (int)indices[i + 1][0];
		int vert3Index = (int)indices[i + 2][0];
		
		Vector3 vert1 = vertices[vert1Index - 1];
		Vector3 vert2 = vertices[vert2Index - 1];
		Vector3 vert3 = vertices[vert3Index - 1];

		//Have 3 verts for a face; link them all together as nodes

		Node* node1 = getNode(vert1);
		Node* node2 = getNode(vert2);
		Node* node3 = getNode(vert3);

		if (node1 == NULL)
		{
			node1 = new Node(vert1);
			nodes.push_back(node1);
		}

		if (node2 == NULL)
		{
			node2 = new Node(vert2);
			nodes.push_back(node2);
		}

		if (node3 == NULL)
		{
			node3 = new Node(vert3);
			nodes.push_back(node3);
		}
		
		node1->addNeighbor(node2);
		node1->addNeighbor(node3);
		addSpring(new Spring(node1, node2, stiffness));
		addSpring(new Spring(node1, node3, stiffness));

		node2->addNeighbor(node1);
		node2->addNeighbor(node3);
		addSpring(new Spring(node2, node1, stiffness));
		addSpring(new Spring(node2, node3, stiffness));

		node3->addNeighbor(node1);
		node3->addNeighbor(node2);
		addSpring(new Spring(node3, node1, stiffness));
		addSpring(new Spring(node3, node2, stiffness));
	}

	//DEBUG
	//Pin some nodes
	nodes[4]->pinned = true;
	nodes[6]->pinned = true;

	//Sort nodes into the same order as the verts
	vector<Node*> orderedNodes;

	for (unsigned int i = 0; i < nodes.size(); i++)
		orderedNodes.push_back(getNode(vertices[i]));

	nodes = orderedNodes;
}

#ifdef _DEBUG
void Softbody::DebugDraw()
{
	if (Debug::Debugging)
	{
		
		Primitive::SetPointSize(6);
		Primitive::SetLineWidth(3);

		Vector3 objPos = getGameObject()->getTransform()->getPosition();

		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			Node* node = nodes[i];
			Vector3 nodePos = node->position;

			Primitive::SetColor(Vector4(0, 0, 1, 1));
			Primitive::DrawPoint(objPos + nodePos);

			for (unsigned int j = 0; j < node->neighbors.size(); j++)
			{
				Primitive::SetColor(Vector4(.5f, .5f, 1, 1));
				Primitive::DrawLine(objPos + nodePos, objPos + node->neighbors[j]->position);
			}
		}
	}
}
#endif

Node* Softbody::getNode(Vector3 pos)
{
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		Node* n = nodes[i];
		if (n->position == pos)
		{
			return n;
		}
	}

	return NULL;
}

void Softbody::addSpring(Spring* spring)
{
	for (unsigned int i = 0; i < springs.size(); i++)
	{
		Spring* s = springs[i];
		if (s->node1 == spring->node1 && s->node2 == spring->node2)
			return;
	}

	springs.push_back(spring);
}

void Softbody::FixedUpdate()
{
	//DEBUG
	if (Input::getKeyDown(KeyCode::e))
	{
		nodes[0]->addForce(Vector3(-0.3f, 0.3f, 0));
	}
	if (Input::getKeyDown(KeyCode::q))
	{
		nodes[3]->addForce(Vector3(0.3f, 0.3f, 0));
	}

	vector<Vector3> newVerts;

	//Apply forces to springs and their connected nodes
	for (unsigned int i = 0; i < springs.size(); i++)
		springs[i]->updateForces();

	//Apply force to nodes
	for (unsigned int i = 0; i < nodes.size(); i++)
		nodes[i]->updateForces();

	//Update mesh data appropriately
	for (unsigned int i = 0; i < nodes.size(); i++)
		newVerts.push_back(nodes[i]->position);
	
	mesh->setVertices(newVerts);
	mesh->bufferChanges();
}

Softbody::~Softbody()
{
	for (unsigned int i = 0; i < nodes.size(); i++)
		delete nodes[i];
	for (unsigned int i = 0; i < springs.size(); i++)
		delete springs[i];
}