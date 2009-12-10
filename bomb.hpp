#ifndef _BOMBER_BOMB
#define _BOMBER_BOMB

#include <cmath>
#include <list>
#include "object.hpp"
#include "level.hpp"

#ifndef M_PI
#define M_PI 3.14159265
#endif

struct Spark
{
    std::list<double> vertex;
    double x, y, z, var_x, var_y, var_z;
    int lasting;
    unsigned long creation_frame;
    Spark(unsigned long frame): x(0.0), y(0.0), z(0.0), creation_frame(frame)
    {
        double length;

        this->vertex.push_back(0.0);
        this->vertex.push_back(0.0);
        this->vertex.push_back(0.0);

        this->var_x = ((double)rand() / RAND_MAX) - 0.5;
        this->var_y = ((double)rand() / RAND_MAX) - 0.5;
        this->var_z = ((double)rand() / RAND_MAX) - 0.15;

        length = sqrt((this->var_x * this->var_x) + (this->var_y * this->var_y) + (this->var_z * this->var_z)) * 230.0;
        if (length != 0.0)
        {
            this->var_x /= length;
            this->var_y /= length;
            this->var_z /= length;
        }

        this->lasting = rand() % 6 + 4;
    }
};

class Bomb: public Object
{
private:
    Level *_level;
    int _player;

    unsigned long _frame;
    unsigned long _explosion_size;
    double _fx, _fy;

    std::list<Spark *> _sparks;
public:
    Bomb(double fx, double fy, unsigned long explosion_size, Level &level, int player);
    virtual ~Bomb() { }

    void explode() { _frame = 100000; }

    void init() { }
    void update();
    void draw();
    std::string type() { return "bomb"; }
};

#endif
