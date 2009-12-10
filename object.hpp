#ifndef _BOMBER_OBJECT
#define _BOMBER_OBJECT

#include <string>

class Object
{
protected:
    unsigned long _x, _y;
    bool _terminate, _unique;
    long _move_x, _move_y;
public:
    Object(): _terminate(false), _unique(false), _move_x(0), _move_y(0) { }

    bool terminate() { return _terminate; }
    bool unique() { return _unique; }

    long move_x() { return _move_x; }
    long move_y() { return _move_y; }
    void position(unsigned long x, unsigned long y) { _x = x; _y = y; _move_x = _move_y = 0; }

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual std::string type() = 0;
};

#endif
