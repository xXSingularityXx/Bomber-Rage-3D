#include <SDL/SDL_opengl.h>
#include "explosion.hpp"
#include "bomb.hpp"
#include "enemy.hpp"

#define NUM_PARTICLES 1000

double random(int max)
{
	return (rand() % 2 == 1) ? (double) (rand() % max) : (double) -(rand() % max);
}

Explosion::Explosion(unsigned long size, int dir, Level &level, int player): _level(&level), _player(player), _dir(dir)
{
    _size = size;
	_numParticles = NUM_PARTICLES;
	_minPart = _numParticles / 2;
	_particles = new Particle[_numParticles]();
}

void Explosion::init()
{
    static int dir_inc[] = {1, 0, 0, 1, -1, 0, 0, -1};

    if (!this->_level->tiles()[this->_level->data()[_x + _y * this->_level->width()]]->walkable())
        this->_terminate = true;

    if (this->_level->tiles()[this->_level->data()[_x + _y * this->_level->width()]]->breakable())
    {
        this->_level->data()[_x + _y * this->_level->width()] = '-';
        this->_terminate = false;
        this->_size = 0;
    }

    if(!this->_terminate)
    {
        if (_size > 0)
        {
            if(_dir == 0){
                for (int i = 0; i < 4; i++)
                    _level->new_object(new Explosion(_size - 1, i + 1, *_level, _player), _x + dir_inc[i * 2], _y + dir_inc[i * 2 + 1]);
            }else{
                _level->new_object(new Explosion(_size - 1, _dir, *_level, _player), _x + dir_inc[(_dir - 1) * 2], _y + dir_inc[(_dir - 1) * 2 + 1]);
            }
        }
    }
}

void Explosion::update()
{

    for (std::list<Object *>::iterator it = this->_level->objects(this->_x, this->_y).begin(); it != this->_level->objects(this->_x, this->_y).end(); it++)
    {
        if (this->_dir > 0 && (*it)->type() == "bomb")
        {
            this->_terminate = true;
            ((Bomb *)(*it))->explode();
        }
        else if ((*it)->type() == "enemy")
            ((Enemy *)(*it))->die();
    }

    for(int i = 0; i < _numParticles; ++i)
    {
        if(!_particles[i].dead){
            _particles[i].pos[0] += random(100) / 500;
            _particles[i].pos[1] += random(100) / 500;
            _particles[i].pos[2] += random(100) / 500;
            if(!_particles[i].dead  && (_particles[i].pos[0] * _particles[i].pos[0]) + (_particles[i].pos[1] * _particles[i].pos[1]) + (_particles[i].pos[2] * _particles[i].pos[2]) > 1.0){
                _particles[i].dead = true;
                _numParticles--;
            }
        }
    }

    if(_numParticles < _minPart){
        this->_terminate = true;
    }
}

void Explosion::draw()
{
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glTranslatef(0.5, 0.0, 0.5);
    glPointSize(10);
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < _numParticles; ++i)
        if(!_particles[i].dead){
            float brightness =
            ((_particles[i].pos[0] * _particles[i].pos[0]) +
            (_particles[i].pos[1] * _particles[i].pos[1]) +
            (_particles[i].pos[2] * _particles[i].pos[2])) / 1.5;
            //GLfloat color[] = {brightness + 0.1, 0.5 - brightness, 0.0, 1.0};
            //glMaterialfv(GL_FRONT, GL_AMBIENT, color);
            glColor3f(1.0, 1.0 - brightness, 0.0);
            glVertex3f(_particles[i].pos[0],_particles[i].pos[1],_particles[i].pos[2]);
        }
    glEnd();

    glPopAttrib();
    glPopMatrix();

}

Explosion::~Explosion()
{
    delete [] _particles;
}
