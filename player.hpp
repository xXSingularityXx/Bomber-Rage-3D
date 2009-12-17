#ifndef _BOMBER_PLAYER
#define _BOMBER_PLAYER

#include "object.hpp"
#include "modelLoader.hpp"

class Player: public Object
{
public:
    typedef enum
    {
        HORIZONTAL,
        VERTICAL
    } axis;


private:
    double _x, _y, _rot, _vrot;
    double _speed;
    axis _current_axis;
    unsigned long _max_bombs, _bombs, _explosion_size;
    Node* _node;
public:
    Player();
    virtual ~Player() { }

    void init();
    void update();
    void draw();
    std::string type() { return "player"; }

    double x() { return _x; }
    double y() { return _y; }
    double rot() { return _rot; }
    double vrot() { return _vrot; }
    axis current_axis() { return _current_axis; }
    unsigned long max_bombs() { return _max_bombs; }
    unsigned long dropped_bombs() { return _bombs; }
    unsigned long explosion_size() { return _explosion_size; }

    void x(double new_x) { _x = new_x; }
    void y(double new_y) { _y = new_y; }
    void rot(double new_rot) { _rot = new_rot; }
    void current_axis(axis new_current_axis) { _current_axis = new_current_axis; }
    void max_bombs(unsigned long new_max_bombs) { _max_bombs = new_max_bombs; }
    void dropped_bombs(unsigned long new_dropped_bombs) { _bombs = new_dropped_bombs; }
    void explosion_size(unsigned long new_explosion_size) { _explosion_size = new_explosion_size; }

    void move(double mul);
    void strafe(double mul);
    void rotate(double angle);
    void vertical_rotate(double angle);
};

#endif
