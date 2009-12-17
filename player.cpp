#include <cmath>
#include "player.hpp"

#ifndef M_PI
    #define M_PI 3.14159265
#endif

Player::Player() : _x(0.0), _y(0.0), _rot(0.0), _vrot(0.0), _speed(0.03), _current_axis(HORIZONTAL), _max_bombs(2), _bombs(0), _explosion_size(2)
{
}

void Player::init()
{
    ObjectManager* objManager = new ObjectManager();
    Texture* tex01 = objManager->createTexture("data/models/bomber.bmp");

	Vector3D v(7.0, 7.0, 7.0);
    this->_node = objManager->createNode("data/models/bomber.MD2");
    this->_node->setScale(v);
    this->_node->setAnimation("stand");

    this->_node->setTexture(tex01);
    delete tex01;
}

void Player::update()
{

}

void Player::draw()
{
    static GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
    static GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

    this->_node->setRotation(90.0 - _rot * 180.0 / M_PI, Vector3D(0.0, 0.0, 1.0));
    this->_node->draw();

    glPopAttrib();
}

void Player::move(double mul)
{
    if (_current_axis == HORIZONTAL)
        _x += _speed * mul;
    else
        _y += _speed * mul;
}

void Player::rotate(double angle)
{
    _rot += angle;
    while (_rot < 0)
        _rot += 2 * M_PI;
    while (_rot >= 2 * M_PI)
        _rot -= 2 * M_PI;
}

void Player::vertical_rotate(double angle)
{
    _vrot -= angle;
    if (_vrot > M_PI/2)
        _vrot = M_PI/2;
    else if (_vrot < -M_PI/2)
        _vrot = -M_PI/2;
}
