#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "Mesh.h"
#include "Vector3D.h"
#include "Texture.h"
#include "ColorRGB.h"
#include "MD2Mesh.h"

class Node
{
public:
	Node();
	Node(Mesh* _mesh);
	Node(MD2::MD2Mesh* _mesh);
	virtual ~Node() {}

	void setPosition(Vector3D _vector);
	void setRotation(float _angle, Vector3D _vector);
	void setScale(Vector3D _vector);
	void setColor(ColorRGB _color);

	void setTexture(Texture* _tex);

	virtual void draw() = 0;
	virtual void advanceFrame(float _dt) = 0;
	virtual void setAnimation(std::string _animName) = 0;

protected:
	Vector3D m_Position;
	Vector3D m_Scale;
	Vector3D m_Rotation;
	unsigned int m_TextureID;
	int m_AngleRotation;

	bool m_isTexturizedNode;

	ColorRGB m_Color;

	Mesh* m_ObjMesh;
	MD2::MD2Mesh* m_md2Mesh;
};

#endif
