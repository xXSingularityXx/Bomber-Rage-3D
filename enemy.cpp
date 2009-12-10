#include <SDL/SDL_opengl.h>
#include "enemy.hpp"

ObjectManager* objManager = new ObjectManager();

Enemy::Enemy(Level &level, double speed): _pos_x(0.5), _pos_y(0.5), _speed(speed), _level(&level)
{
    this->_dir = rand() % 4;

    Texture* tex01 = objManager->createTexture("pknight.BMP");


	Vector3D v(0.02,0.02,0.02);
    this->_node = objManager->createNode("pknight.MD2");
    this->_node->setScale(v);
    this->_node->setAnimation("stand");

    this->_node->setTexture(tex01);
    delete tex01;
}

void Enemy::init()
{

}

void Enemy::update()
{
    static int dir_inc[] = {1, 0, 0, 1, -1, 0, 0, -1};

    if (!(this->_level->walkable(this->_x + 1, this->_y)) && !(this->_level->walkable(this->_x, this->_y + 1)) &&
        !(this->_level->walkable(this->_x - 1, this->_y)) && !(this->_level->walkable(this->_x, this->_y - 1)))
    {
        this->_pos_x = this->_pos_y = 0.5;
        this->_dir = 3;
    }
    else
    {
        //this->_node->setAnimation("WALKING");
        double pos_aux;
        int dir1 = this->_dir;
        int count = 0;
        bool repeat = true;

        pos_aux = (this->_dir == 0 || this->_dir == 2)? this->_pos_x: this->_pos_y;
        this->_pos_x += this->_speed * (double)dir_inc[this->_dir * 2];
        this->_pos_y += this->_speed * (double)dir_inc[this->_dir * 2 + 1];

        if (rand() % 7 ==  0 &&
            ((this->_dir == 0 && pos_aux < 0.5 && this->_pos_x >= 0.5) ||
            (this->_dir == 1 && pos_aux < 0.5 && this->_pos_y >= 0.5) ||
            (this->_dir == 2 && pos_aux > 0.5 && this->_pos_x <= 0.5) ||
            (this->_dir == 3 && pos_aux > 0.5 && this->_pos_y <= 0.5)))
        {
            this->_dir = rand() % 4;
            if (this->_dir % 2 != dir1 % 2)
                if (this->_dir == 0 || this->_dir == 2)
                    this->_pos_y = 0.5;
                else
                    this->_pos_x = 0.5;
        }

        do
        {
            if ((this->_dir == 0 && (count || (pos_aux < 0.5 && this->_pos_x >= 0.5)) && !(this->_level->walkable(this->_x + 1, this->_y))) ||
                (this->_dir == 1 && (count || (pos_aux < 0.5 && this->_pos_y >= 0.5)) && !(this->_level->walkable(this->_x, this->_y + 1))) ||
                (this->_dir == 2 && (count || (pos_aux > 0.5 && this->_pos_x <= 0.5)) && !(this->_level->walkable(this->_x - 1, this->_y))) ||
                (this->_dir == 3 && (count || (pos_aux > 0.5 && this->_pos_y <= 0.5)) && !(this->_level->walkable(this->_x, this->_y - 1))))
                if (count == 0)
                {
                    if (this->_dir == 0 || this->_dir == 2)
                        this->_pos_x = 0.5;
                    else
                        this->_pos_y = 0.5;
                    this->_dir += 2 * (rand() % 2) - 1;
                }
                else if (count == 1)
                    this->_dir += 2;
                else
                {
                    this->_dir = dir1 + 2;
                    repeat = false;
                }
            else if ((this->_dir == 0 && pos_aux >= 0.5 && this->_pos_x >= 0.5 && !(this->_level->walkable(this->_x + 1, this->_y))) ||
                (this->_dir == 1 && pos_aux >= 0.5 && this->_pos_y >= 0.5 && !(this->_level->walkable(this->_x, this->_y + 1))) ||
                (this->_dir == 2 && pos_aux <= 0.5 && this->_pos_x <= 0.5 && !(this->_level->walkable(this->_x - 1, this->_y))) ||
                (this->_dir == 3 && pos_aux <= 0.5 && this->_pos_y <= 0.5 && !(this->_level->walkable(this->_x, this->_y - 1))))
            {
                this->_dir += 2;
                repeat = false;
            }
            else
                repeat = false;

            if (this->_dir < 0)
                this->_dir = 3;
            else if (this->_dir > 3)
                this->_dir = 0;
            count++;
        } while (repeat);

        if (this->_pos_x < 0.0)
        {
            this->_pos_x += 1.0;
            this->_move_x = -1;
        }
        else if (this->_pos_x >= 1.0)
        {
            this->_pos_x -= 1.0;
            this->_move_x = 1;
        }
        if (this->_pos_y < 0.0)
        {
            this->_pos_y += 1.0;
            this->_move_y = -1;
        }
        else if (this->_pos_y >= 1.0)
        {
            this->_pos_y -= 1.0;
            this->_move_y = 1;
        }
    }
    this->_node->advanceFrame(0.005);

}

void Enemy::draw()
{
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_DEPTH_TEST);

    glTranslatef(this->_pos_x, 0.4, this->_pos_y);
    glRotatef(90.0 * (1 - this->_dir), 0.0, 1.0, 0.0);
    glFrontFace(GL_CW);
    this->_node->draw();

    glPopMatrix();
    glPopAttrib();
}

void Enemy::die()
{
    this->_terminate = true;
}
