#ifndef ANIMATED_NODE_H
#define ANIMATED_NODE_H

#include "Node.h"
#include "Mesh.h"
#include "MD2Normals.h"
#include <SDL/SDL_opengl.h>

class AnimatedNode : public Node
{
public:
	AnimatedNode(MD2::MD2Mesh* _mesh) : Node(_mesh){}
	~AnimatedNode(){}

	void setAnimation(std::string _animName);
	void advanceFrame(float _dt);

	virtual void draw();
};



#endif
