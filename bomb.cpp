#include <algorithm>
#include <cmath>
#include <SDL/SDL_opengl.h>
#include "bomb.hpp"
#include "explosion.hpp"

Bomb::Bomb(double fx, double fy, unsigned long explosion_size, Level &level, int player): _level(&level), _player(player), _frame(0), _explosion_size(explosion_size)
{
    this->_fx = fx - (unsigned long)fx;
    this->_fy = fy - (unsigned long)fy;

    this->_unique = true;
}

void Bomb::update()
{
    std::list<std::list<Spark *>::iterator> to_delete;

    this->_frame++;

    for (std::list<Spark *>::iterator it = this->_sparks.begin(); it != this->_sparks.end(); it++)
        if ((long)(this->_frame - (*it)->creation_frame) > (*it)->lasting)
            if (!(*it)->vertex.empty())
            {
                (*it)->vertex.pop_front();
                (*it)->vertex.pop_front();
                (*it)->vertex.pop_front();
            }
            else
                to_delete.push_back(it);
        else
        {
            (*it)->vertex.push_back((*it)->x += (*it)->var_x);
            (*it)->vertex.push_back((*it)->y += (*it)->var_y);
            (*it)->vertex.push_back((*it)->z += (*it)->var_z);

            if ((*it)->vertex.size() > 15)
            {
                (*it)->vertex.pop_front();
                (*it)->vertex.pop_front();
                (*it)->vertex.pop_front();
            }

            (*it)->var_z -= 0.0003;
        }

    for (std::list<std::list<Spark *>::iterator>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
    {
        delete **it;
        this->_sparks.erase(*it);
    }

//if (this->_frame % 5 == 4)
    for (int i = 0; i < 25; i++)
        this->_sparks.push_back(new Spark(this->_frame));

    if (this->_frame > 60 * M_PI + 40)
    {
        this->_terminate = true;
        this->_level->new_object(new Explosion(this->_explosion_size, 0, *this->_level, this->_player), this->_x, this->_y);
        if (this->_player >= 0)
            this->_level->player(this->_player).dropped_bombs(this->_level->player(this->_player).dropped_bombs() - 1);
    }
}

void Bomb::draw()
{
    double radius;
    static GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
    static GLfloat gray[] = {0.1, 0.1, 0.1, 1.0};
    static GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    GLUquadric *quad = gluNewQuadric();

    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    if (this->_frame < 40)
    {
        double k = (double)this->_frame * 0.025;
        radius = sin(k * (M_PI * 0.5)) * (0.25 + 1.0 / 35.0);
        glTranslatef(this->_fx + (0.5 - this->_fx) * k,
            cos(k * M_PI * 0.5) * (0.5 - (0.25 + 1.0 / 35.0)) + (0.25 + 1.0 / 35.0),
            this->_fy + (0.5 - this->_fy) * k);
    }
    else
    {
        radius = 0.25 + cos(((double)this->_frame - 40) / 10.0) / 35.0;
        glTranslatef(0.5, radius, 0.5);
    }
    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT, GL_AMBIENT, black);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
    gluSphere(quad, radius, 100, 100);

    glTranslatef(0.0, radius * 0.94, 0.0);
    glRotatef(90.0, -1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, -radius * 0.94);
    glRotatef(30.0, 1.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, radius * 0.94);
    glMaterialfv(GL_FRONT, GL_AMBIENT, gray);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
    gluCylinder(quad, 0.19 * radius, 0.17 * radius, 0.13 * radius, 50, 5);

    glTranslatef(0.0, 0.0, radius * 0.13);
    gluDisk(quad, 0.0, 0.17 * radius, 50, 50);

    glMaterialfv(GL_FRONT, GL_AMBIENT, gray);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, gray);
    //glTranslatef(0.0, 0.0, -radius * 0.04);
    gluCylinder(quad, 0.02 * radius, 0.02 * radius, (60.0 * M_PI + 40.0 - (GLfloat)this->_frame) * 0.0002, 10, 1);

    glDisable(GL_LIGHTING);
    glColor3f(1.0, 0.9, 0.0);
    glTranslatef(0.0, 0.0, (60.0 * M_PI + 40.0 - (GLfloat)this->_frame) * 0.0002);
    gluSphere(quad, 0.02 * radius, 10, 10);

    glLineWidth(1.0);
    for (std::list<Spark *>::iterator it = this->_sparks.begin(); it != this->_sparks.end(); it++)
    {
        double x, y, z;
        unsigned int vertex = ((long)(this->_frame - (*it)->creation_frame) <= (*it)->lasting)? (*it)->vertex.size() / 3: 5;
        glBegin(GL_LINE_STRIP);
        for (std::list<double>::iterator it2 = (*it)->vertex.begin(); it2 != (*it)->vertex.end(); it2++)
        {
            x = *it2;
            y = *(++it2);
            z = *(++it2);
            glColor3f(1.0, std::max(std::min(1.5 - (double)(this->_frame - (*it)->creation_frame - vertex + 1) / 5.0, 0.95), 0.2), 0.0);
            glVertex3f(x, y, z);
            vertex--;
        }
        glEnd();
    }

    glPopAttrib();
    glPopMatrix();
    gluDeleteQuadric(quad);
}
