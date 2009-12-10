
#include "Node.h"

Node::Node()
{
	m_Position = Vector3D(0,0,0);
	m_Rotation = Vector3D(0,0,0);
	m_Scale = Vector3D(1,1,1);

	m_Color.r = 0.5;
	m_Color.g = 0.5;
	m_Color.b = 0.5;

	m_AngleRotation=0;
	m_isTexturizedNode=false;
}

Node::Node(Mesh* _mesh)
{
	m_ObjMesh = _mesh;

	m_Position = Vector3D(0,0,0);
	m_Rotation = Vector3D(0,0,0);
	m_Scale = Vector3D(1,1,1);

	m_AngleRotation=0;
	m_isTexturizedNode=false;
}

Node::Node(MD2::MD2Mesh* _mesh)
{
	m_md2Mesh = _mesh;

	m_Position = Vector3D(0,0,0);
	m_Rotation = Vector3D(0,0,0);
	m_Scale = Vector3D(1,1,1);

	m_AngleRotation=0;
	m_isTexturizedNode=false;
}
/*
	set a position
*/
void Node::setPosition(Vector3D _vector)
{
	m_Position = _vector;
}

/*
	rotate the node
*/
void Node::setRotation(float _angle, Vector3D _vector)
{
	m_Rotation = _vector;
	m_AngleRotation = (int) _angle;
}

void Node::setTexture(Texture* _tex)
{
	if (_tex->m_TextureId!=NULL)
	{
		m_isTexturizedNode=true;
		m_TextureID = _tex->m_TextureId;
	}
	else
		std::cout << "Error: not a valid texture";
}

/*
	scale the Node in the x,y and z axis
*/
void Node::setScale(Vector3D _vector)
{
	m_Scale = _vector;
}

void Node::setColor(ColorRGB _color)
{
	m_Color = _color;
}

