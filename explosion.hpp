#ifndef _BOMBER_EXPLOSION
#define _BOMBER_EXPLOSION

#include "object.hpp"
#include "level.hpp"

struct Particle{
	double pos[3];
	bool dead;
	Particle() { pos[0] = pos[1] = pos[2] = dead = 0; }
};

class Explosion: public Object
{
private:
    Level *_level;
    int _player;
    int _dir;
	Particle* _particles;
	int       _numParticles;
	int       _minPart;
	unsigned long _size;

    //unsigned int _frame;
public:
    Explosion(unsigned long size, int dir, Level &level, int player);

    void init();
    void update();
    void draw();
    std::string type() { return "explosion"; }

    virtual ~Explosion();
};

#endif
