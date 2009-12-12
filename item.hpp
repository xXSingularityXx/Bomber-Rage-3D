#ifndef _BOMBER_ITEM
#define _BOMBER_ITEM

#include "level.hpp"
#include "object.hpp"

class Item: public Object
{
public:
    typedef enum
    {
        bomb,
        fire,
        skate,

        hand,
        glove,
        boot,

        remote_bomb,
        wall_pass_through,
        bomb_pass_through,
        spike_bomb,
        mine,

        heart,
        vest,
        super_fire,

        life,
        time,

        slipper,
        disease
    } ITEM;
private:
    bool _hide;
    Level *_level;
public:
    Item(Level &level);
    virtual ~Item() { }

    void init();
    void update();
    void draw();
    std::string type() { return "item"; }

    void destroy();
};

#endif
