#ifndef _BOMBER_ENEMY
#define _BOMBER_ENEMY

#include "level.hpp"
#include "object.hpp"


#include "modelLoader/ImageLoader.h"
#include "modelLoader/Vector3D.h"
#include "modelLoader/ObjectManager.h"
#include "modelLoader/Node.h"
#include "modelLoader/StaticNode.h"
#include "modelLoader/Texture.h"


class Enemy: public Object
{
private:
    int _dir;
    double _pos_x, _pos_y;
    double _speed;
    Level *_level;
    Node* _node;
public:
    Enemy(Level &level, double speed = 0.005);
    virtual ~Enemy() { }

    void init();
    void update();
    void draw();
    std::string type() { return "enemy"; }

    void die();
};

#endif
