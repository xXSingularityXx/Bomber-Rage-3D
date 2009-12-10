#ifndef STATIC_NODE_H
#define STATIC_NODE_H

#include <iostream>
//#include <GL/glut.h>
#include <SDL/SDL_opengl.h>
#include "Mesh.h"
#include "Vector3D.h"
#include "Texture.h"
#include "ColorRGB.h"
#include "Node.h"

class StaticNode : public Node
{
public:
	StaticNode(Mesh* _mesh) : Node(_mesh){}
	~StaticNode(){}

	virtual void draw();
	virtual void advanceFrame(float _dt){}
	virtual void setAnimation(std::string _animName){}

};

#endif
