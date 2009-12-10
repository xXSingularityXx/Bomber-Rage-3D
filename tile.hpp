#ifndef _BOMBER_TILE
#define _BOMBER_TILE

#include <vector>
#include <SDL/SDL_opengl.h>

class Tile
{
public:
    static void load_tiles(const char *filename, const std::vector<GLuint> &textures, Tile *tiles[256]);
private:
    GLuint _display_list;
    unsigned long _probabilities_size;
    double *_probabilities;
    unsigned char *_clones;

    bool _walkable, _breakable, _enemy;
public:
    Tile();
    ~Tile();

    bool walkable() { return _walkable; }
    bool breakable() { return _breakable; }

    unsigned char draw(bool &new_enemy);
};

#endif
