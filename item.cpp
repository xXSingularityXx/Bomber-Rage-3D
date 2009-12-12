#include <SDL/SDL_opengl.h>
#include "item.hpp"

Item::Item(Level &level): _hide(true), _level(&level)
{ }

void Item::init()
{

}

void Item::update()
{

}

void Item::draw()
{
    //if (!this->_hide)
    {
        static GLfloat color[] = {1.0, 1.0, 0.0, 1.0};
        GLUquadric *quad = gluNewQuadric();

        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glTranslatef(0.5, 0.65, 0.5);

        glDisable(GL_TEXTURE_2D);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        gluSphere(quad, 0.5, 100, 100);

        glPopMatrix();
        glPopAttrib();
        gluDeleteQuadric(quad);
    }
}

void Item::destroy()
{
    this->_terminate = true;
}
