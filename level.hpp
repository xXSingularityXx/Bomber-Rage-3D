#ifndef _BOMBER_LEVEL
#define _BOMBER_LEVEL

#include <list>
#include <vector>
#include <SDL/SDL_opengl.h>
#include <fstream>
#include "player.hpp"
#include "object.hpp"
#include "tile.hpp"

class Level
{
private:
    std::vector<GLuint> _textures;
    Tile *_tiles[256];
    std::list<Object *> *_objects;

    unsigned char *_level;
    unsigned long _width, _height;

    int _num_players;
    Player _players[5];
public:
    Level(int num_players);
    ~Level();

    unsigned char *data() { return _level; }
    unsigned long width() { return _width; }
    unsigned long height() { return _height; }
    Player &player(int number) { return _players[number]; }
    Tile **tiles() { return _tiles; }
    std::list<Object *> &objects(unsigned long x, unsigned long y) { return _objects[x + y * _width]; }

    void load(const char *filename, unsigned long number);
    void draw();
    void update();

    void new_object(Object *object, unsigned long x, unsigned long y);
    void add_texture(const char *filename, unsigned long index);

    bool walkable(unsigned long x, unsigned long y);
    void move_player(int number, double fmul, double smul);
};

#endif
