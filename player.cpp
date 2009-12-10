#include <cmath>
#include "player.hpp"

#ifndef M_PI
#define M_PI 3.14159265
#endif

Player::Player() : _x(0.0), _y(0.0), _rot(0.0), _vrot(0.0), _speed(0.03), _current_axis(HORIZONTAL), _max_bombs(2), _bombs(0), _explosion_size(2)
{ }

void Player::init()
{

}

void Player::update()
{

}

void Player::draw()
{

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
