#include <fstream>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <GL/gl.h>

#include "level.hpp"
#include "texture.hpp"
#include "enemy.hpp"
#include "item.hpp"

#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_CLAMP_TO_EDGE 0x812F
#endif

#define SKYBOX_RADIUS (60.0f)

#ifndef M_PI
#define M_PI 3.14159265
#endif

Level::Level(int num_players): _level(NULL), _num_players(num_players)
{
    for (int i = 0; i < 256; i++)
        this->_tiles[i] = NULL;
}

Level::~Level()
{
    if (this->_level != NULL)
        delete[] this->_level;

    for (std::vector<GLuint>::iterator it = this->_textures.begin();
        it != this->_textures.end(); it++)
        glDeleteTextures(1, &(*it));

    for (int i = 0; i < 256; i++)
        if (this->_tiles[i] != NULL)
            delete this->_tiles[i];
}

void Level::load(const char *filename, unsigned long number)
{
    unsigned char command, item;
    unsigned long k = 0, count;
    std::fstream level_file;
    std::string aux_filename;
    std::list<Item *> items;
    std::vector<unsigned long> breakable_tiles;

    level_file.open(filename, std::fstream::in);
    while (true)
    {
        do
            level_file >> command;
        while (!level_file.eof() && command != '{');

        if (level_file.eof())
            break;
        level_file >> k;

        if (k == number)
            do
            {
                level_file >> command;
                switch (command)
                {
                case 's': // Store texture
                    level_file >> k;
                    std::getline(level_file, aux_filename);
                    this->add_texture(aux_filename.c_str() + 1, k);
                    break;
                case 't': // Tiles file
                    std::getline(level_file, aux_filename);
                    Tile::load_tiles(aux_filename.c_str(), this->_textures, this->_tiles);
                    break;
                case 'd': // Dimension (width height)
                    level_file >> this->_width >> this->_height;
                    break;
                case 'l': // Level Data
                    this->_level = new unsigned char[this->_width * this-> _height];
                    this->_objects = new std::list<Object *>[this->_width * this-> _height];
                    for (k = 0; k < this->_width * this-> _height; k++)
                    {
                        level_file >> this->_level[k];
                        if (this->_level[k] == '.')
                            breakable_tiles.push_back(k);
                        if (_level[k] >= '1' && this->_level[k] <= '5')
                        {
                            this->_players[this->_level[k] - '1'].x((double)(k % this->_width) + 0.5);
                            this->_players[this->_level[k] - '1'].y((double)(k / this->_width) + 0.5);
                            this->_level[k] = '-';
                        }
                    }
                    break;
                case 'i': // Item
                    level_file >> item >> count;
                    switch (item)
                    {
                        case 'b': // bomb
                            items.push_back(new Item(*this));
                            break;
                        case 'f': // fire
                            items.push_back(new Item(*this));
                            break;
                        case 's': // skate
                            items.push_back(new Item(*this));
                            break;
                    }
                    break;
                }
            } while (command != '}');
    }
    level_file.close();

    for (std::list<Item *>::iterator it = items.begin(); it != items.end(); it++)
    {
        if (breakable_tiles.size() == 0)
            exit(1234);
        unsigned long i = rand() % breakable_tiles.size();
        this->new_object(*it, breakable_tiles[i] % this->_width, breakable_tiles[i] / this->_width);
        breakable_tiles.erase(breakable_tiles.begin() + i);
    }

    for (k = 0; k < this->_width * this-> _height; k++)
    {
        bool new_enemy;
        unsigned char new_tile;

        if ((new_tile = this->_tiles[this->_level[k]]->init(new_enemy)) != 0)
            this->_level[k] = new_tile;
        if (new_enemy)
            this->new_object(new Enemy(*this), k % this->_width, k / this->_width);
    }

    for (int i = 0; i < this->_num_players; i++)
        this->_players[i].init();
}

void Level::draw()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glBindTexture(GL_TEXTURE_2D, this->_textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
        glTexCoord2d(0.0f, 0.0f);
        glVertex3f(-SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS);
        glTexCoord2d(1.0f, 0.0f);
        glVertex3f( SKYBOX_RADIUS, SKYBOX_RADIUS, -SKYBOX_RADIUS);
        glTexCoord2d(1.0f, 1.0f);
        glVertex3f( SKYBOX_RADIUS, SKYBOX_RADIUS,  SKYBOX_RADIUS);
        glTexCoord2d(0.0f, 1.0f);
        glVertex3f(-SKYBOX_RADIUS, SKYBOX_RADIUS,  SKYBOX_RADIUS);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, this->_textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
        glTexCoord2d(0.0f, 1.0f);
        glVertex3f(-SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS);
        glTexCoord2d(0.0f, 0.0f);
        glVertex3f(-SKYBOX_RADIUS,  SKYBOX_RADIUS, SKYBOX_RADIUS);
        glTexCoord2d(1.0f, 0.0f);
        glVertex3f( SKYBOX_RADIUS,  SKYBOX_RADIUS, SKYBOX_RADIUS);
        glTexCoord2d(1.0f, 1.0f);
        glVertex3f( SKYBOX_RADIUS, -SKYBOX_RADIUS, SKYBOX_RADIUS);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, this->_textures[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
        glTexCoord2d(1.0f, 1.0f);
        glVertex3f(-SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS);
        glTexCoord2d(0.0f, 1.0f);
        glVertex3f( SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS);
        glTexCoord2d(0.0f, 0.0f);
        glVertex3f( SKYBOX_RADIUS,  SKYBOX_RADIUS, -SKYBOX_RADIUS);
        glTexCoord2d(1.0f, 0.0f);
        glVertex3f(-SKYBOX_RADIUS,  SKYBOX_RADIUS, -SKYBOX_RADIUS);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, this->_textures[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
        glTexCoord2d(1.0f, 1.0f);
        glVertex3f(SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS);
        glTexCoord2d(0.0f, 1.0f);
        glVertex3f(SKYBOX_RADIUS, -SKYBOX_RADIUS,  SKYBOX_RADIUS);
        glTexCoord2d(0.0f, 0.0f);
        glVertex3f(SKYBOX_RADIUS,  SKYBOX_RADIUS,  SKYBOX_RADIUS);
        glTexCoord2d(1.0f, 0.0f);
        glVertex3f(SKYBOX_RADIUS,  SKYBOX_RADIUS, -SKYBOX_RADIUS);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, this->_textures[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_EXT_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_EXT_CLAMP_TO_EDGE);
    glBegin(GL_QUADS);
        glTexCoord2d(0.0f, 1.0f);
        glVertex3f(-SKYBOX_RADIUS, -SKYBOX_RADIUS, -SKYBOX_RADIUS);
        glTexCoord2d(0.0f, 0.0f);
        glVertex3f(-SKYBOX_RADIUS,  SKYBOX_RADIUS, -SKYBOX_RADIUS);
        glTexCoord2d(1.0f, 0.0f);
        glVertex3f(-SKYBOX_RADIUS,  SKYBOX_RADIUS,  SKYBOX_RADIUS);
        glTexCoord2d(1.0f, 1.0f);
        glVertex3f(-SKYBOX_RADIUS, -SKYBOX_RADIUS,  SKYBOX_RADIUS);
    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);

    for (int i = 0; i < this->_num_players; i++)
    {
        glPushMatrix();
        glTranslatef(this->_players[i].x(), 0.0, this->_players[i].y());
        this->_players[i].draw();
        glPopMatrix();
    }
    for (unsigned long k = 0; k < this->_width * this->_height; k++)
    {
        glPushMatrix();
        glTranslatef((double)(k % this->_width), 0.0, (double)(k / this->_width));
        this->_tiles[this->_level[k]]->draw();
        for (std::list<Object *>::iterator it = this->_objects[k].begin();
            it != this->_objects[k].end(); it++)
            (*it)->draw();
        glPopMatrix();
    }
}

void Level::update()
{
    typedef std::pair<long, std::list<Object *>::iterator> pair1;
    typedef std::pair<long, long> pair2;
    typedef std::pair<pair1, pair2> move_pair;

    std::list<std::list<Object *>::iterator> to_delete;
    std::list<move_pair> to_move;

    for (int i = 0; i < this->_num_players; i++)
        this->_players[i].update();
    for (unsigned long k = 0; k < this->_width * this->_height; k++)
    {
        to_delete.clear();
        for (std::list<Object *>::iterator it = this->_objects[k].begin();
            it != this->_objects[k].end(); it++)
        {
            (*it)->update();
            if ((*it)->terminate())
                to_delete.push_back(it);
            else if ((*it)->move_x() != 0 || (*it)->move_y() != 0)
                to_move.push_back(move_pair(pair1(k, it), pair2((*it)->move_x(), (*it)->move_y())));
        }

        for (std::list<std::list<Object *>::iterator>::iterator it = to_delete.begin();
            it != to_delete.end(); it++)
        {
            delete **it;
            this->_objects[k].erase(*it);
        }
    }

    for (std::list<move_pair>::iterator it = to_move.begin(); it != to_move.end(); it++)
    {
        (*(it->first.second))->position(it->first.first % this->_width + it->second.first, it->first.first / this->_width + it->second.second);
        this->_objects[it->first.first + it->second.first + it->second.second * this->_width].push_back(*(it->first.second));
        this->_objects[it->first.first].erase(it->first.second);
    }
}

void Level::new_object(Object *object, unsigned long x, unsigned long y)
{
    object->position(x, y);
    object->init();
    if (!object->terminate())
        this->_objects[x + y * this->_width].push_back(object);
}

void Level::add_texture(const char *filename, unsigned long index)
{
    if (this->_textures.size() <= index)
        this->_textures.resize(index + 1, false);
    this->_textures[index] = load_texture(filename);
}

bool Level::walkable(unsigned long x, unsigned long y)
{
    if (!this->_tiles[this->_level[x + y * this->_width]]->walkable())
        return false;
    for (std::list<Object *>::iterator it = this->_objects[x + y * this->_width].begin();
        it != this->_objects[x + y * this->_width].end(); it++)
        if ((*it)->unique())
            return false;
    return true;
}

void Level::move_player(int number, double fmul, double smul)
{
    bool repeat = true;
    long ix, iy, irot;
    double aux;

    ix = (long)(this->_players[number].x());
    iy = (long)(this->_players[number].y());
    irot = (long)(16.0 * this->_players[number].rot() / M_PI);

    if (smul < 0.0)
    {
        irot -= 8;
        if (fmul > 0.0)
            irot += 4;
        else if (fmul < 0.0)
            irot -= 4;
    }
    else if (smul > 0.0)
    {
        irot += 8;
        if (fmul > 0.0)
            irot -= 4;
        else if (fmul < 0.0)
            irot += 4;
    }
    else if (fmul < 0.0)
        irot += 16;

    if (irot < 0)
        irot += 32;
    else if (irot > 31)
        irot -= 32;

    do
        switch (irot)
        {
        case 0:
        case 1:
        case 30:
        case 31:
            if (this->walkable(ix + 1, iy))
            {
                if (this->_players[number].y() - (double)iy >= 0.47 &&
                    this->_players[number].y() - (double)iy <= 0.53)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                else if (this->_players[number].y() - (double)iy < 0.47)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                else if (this->_players[number].y() - (double)iy > 0.53)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
            }
            else
            {
                aux = this->_players[number].x() - (double)ix;
                if (this->_players[number].x() - (double)ix < 0.5)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                else if (this->walkable(ix + 1, iy - 1) &&
                    this->_players[number].y() - (double)iy < 0.34)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
                else if (this->walkable(ix + 1, iy + 1) &&
                    this->_players[number].y() - (double)iy > 0.66)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                if (aux < 0.5 && this->_players[number].x() - (double)ix > 0.5)
                    this->_players[number].x((double)ix + 0.5);
            }
            repeat = false;
            break;

        case 2:
        case 3:
        case 4:
        case 5:
            if ((this->walkable(ix + 1, iy) || this->_players[number].x() - (double)ix < 0.5) &&
                !this->walkable(ix, iy + 1))
            {
                irot = 0;
                break;
            }
            else if (!this->walkable(ix + 1, iy) && (this->walkable(ix, iy + 1) ||
                this->_players[number].y() - (double)iy < 0.5))
            {
                irot = 6;
                break;
            }
            else if (this->walkable(ix + 1, iy) && this->walkable(ix, iy + 1))
            {
                if (this->_players[number].current_axis() == Player::HORIZONTAL)
                {
                    aux = this->_players[number].x() - (double)ix;
                    this->_players[number].move(fmul * fmul + smul * smul);
                    if (aux < 0.5 && this->_players[number].x() - (double)ix >= 0.5)
                    {
                        this->_players[number].current_axis(Player::VERTICAL);
                        this->_players[number].x((double)ix + 0.5);
                        this->_players[number].y((double)iy + 0.505);
                    }
                }
                else
                {
                    aux = this->_players[number].y() - (double)iy;
                    this->_players[number].move(fmul * fmul + smul * smul);
                    if (aux < 0.5 && this->_players[number].y() - (double)iy >= 0.5)
                    {
                        this->_players[number].current_axis(Player::HORIZONTAL);
                        this->_players[number].y((double)iy + 0.5);
                        this->_players[number].x((double)ix + 0.505);
                    }
                }
            }
            repeat = false;
            break;

        case 6:
        case 7:
        case 8:
        case 9:
            if (this->walkable(ix, iy + 1))
            {
                if (this->_players[number].x() - (double)ix >= 0.47 &&
                    this->_players[number].x() - (double)ix <= 0.53)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                else if (this->_players[number].x() - (double)ix < 0.47)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                else if (this->_players[number].x() - (double)ix > 0.53)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
            }
            else
            {
                aux = this->_players[number].y() - (double)iy;
                if (this->_players[number].y() - (double)iy < 0.5)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                else if (this->walkable(ix - 1, iy + 1) &&
                    this->_players[number].x() - (double)ix < 0.34)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
                else if (this->walkable(ix + 1, iy + 1) &&
                    this->_players[number].x() - (double)ix > 0.66)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                if (aux < 0.5 && this->_players[number].y() - (double)iy > 0.5)
                    this->_players[number].y((double)iy + 0.5);
            }
            repeat = false;
            break;

        case 10:
        case 11:
        case 12:
        case 13:
            if (!this->walkable(ix - 1, iy) && (this->walkable(ix, iy + 1) ||
                this->_players[number].y() - (double)iy < 0.5))
            {
                irot = 6;
                break;
            }
            else if ((this->walkable(ix - 1, iy) || this->_players[number].x() - (double)ix > 0.5) &&
                !this->walkable(ix, iy + 1))
            {
                irot = 14;
                break;
            }
            else if (this->walkable(ix - 1, iy) && this->walkable(ix, iy + 1))
            {
                if (this->_players[number].current_axis() == Player::HORIZONTAL)
                {
                    aux = this->_players[number].x() - (double)ix;
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                    if (aux > 0.5 && this->_players[number].x() - (double)ix <= 0.5)
                    {
                        this->_players[number].current_axis(Player::VERTICAL);
                        this->_players[number].x((double)ix + 0.5);
                        this->_players[number].y((double)iy + 0.505);
                    }
                }
                else
                {
                    aux = this->_players[number].y() - (double)iy;
                    this->_players[number].move(fmul * fmul + smul * smul);
                    if (aux < 0.5 && this->_players[number].y() - (double)iy >= 0.5)
                    {
                        this->_players[number].current_axis(Player::HORIZONTAL);
                        this->_players[number].y((double)iy + 0.5);
                        this->_players[number].x((double)ix + 0.495);
                    }
                }
            }
            repeat = false;
            break;

        case 14:
        case 15:
        case 16:
        case 17:
            if (this->walkable(ix - 1, iy))
            {
                if (this->_players[number].y() - (double)iy >= 0.47 &&
                    this->_players[number].y() - (double)iy <= 0.53)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
                else if (this->_players[number].y() - (double)iy < 0.47)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                else if (this->_players[number].y() - (double)iy > 0.53)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
            }
            else
            {
                aux = this->_players[number].x() - (double)ix;
                if (this->_players[number].x() - (double)ix > 0.5)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
                else if (this->walkable(ix - 1, iy - 1) &&
                    this->_players[number].y() - (double)iy < 0.34)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
                else if (this->walkable(ix - 1, iy + 1) &&
                    this->_players[number].y() - (double)iy > 0.66)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                if (aux > 0.5 && this->_players[number].x() - (double)ix < 0.5)
                    this->_players[number].x((double)ix + 0.5);
            }
            repeat = false;
            break;

        case 18:
        case 19:
        case 20:
        case 21:
            if ((this->walkable(ix - 1, iy) || this->_players[number].x() - (double)ix > 0.5) &&
                !this->walkable(ix, iy - 1))
            {
                irot = 14;
                break;
            }
            else if (!this->walkable(ix - 1, iy) && (this->walkable(ix, iy - 1) ||
                this->_players[number].y() - (double)iy > 0.5))
            {
                irot = 22;
                break;
            }
            else if (this->walkable(ix - 1, iy) && this->walkable(ix, iy - 1))
            {
                if (this->_players[number].current_axis() == Player::HORIZONTAL)
                {
                    aux = this->_players[number].x() - (double)ix;
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                    if (aux > 0.5 && this->_players[number].x() - (double)ix <= 0.5)
                    {
                        this->_players[number].current_axis(Player::VERTICAL);
                        this->_players[number].x((double)ix + 0.5);
                        this->_players[number].y((double)iy + 0.495);
                    }
                }
                else
                {
                    aux = this->_players[number].y() - (double)iy;
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                    if (aux > 0.5 && this->_players[number].y() - (double)iy <= 0.5)
                    {
                        this->_players[number].current_axis(Player::HORIZONTAL);
                        this->_players[number].y((double)iy + 0.5);
                        this->_players[number].x((double)ix + 0.495);
                    }
                }
            }
            repeat = false;
            break;

        case 22:
        case 23:
        case 24:
        case 25:
            if (this->walkable(ix, iy - 1))
            {
                if (this->_players[number].x() - (double)ix >= 0.47 &&
                    this->_players[number].x() - (double)ix <= 0.53)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
                else if (this->_players[number].x() - (double)ix < 0.47)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                else if (this->_players[number].x() - (double)ix > 0.53)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
            }
            else
            {
                aux = this->_players[number].y() - (double)iy;
                if (this->_players[number].y() - (double)iy > 0.5)
                {
                    this->_players[number].current_axis(Player::VERTICAL);
                    this->_players[number].x((double)ix + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
                else if (this->walkable(ix - 1, iy - 1) && this->_players[number].x() - (double)ix < 0.34)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                }
                else if (this->walkable(ix + 1, iy - 1) && this->_players[number].x() - (double)ix > 0.66)
                {
                    this->_players[number].current_axis(Player::HORIZONTAL);
                    this->_players[number].y((double)iy + 0.5);
                    this->_players[number].move(fmul * fmul + smul * smul);
                }
                if (aux > 0.5 && this->_players[number].y() - (double)iy < 0.5)
                    this->_players[number].y((double)iy + 0.5);
            }
            repeat = false;
            break;

        case 26:
        case 27:
        case 28:
        case 29:
            if (!this->walkable(ix + 1, iy) && (this->walkable(ix, iy - 1) ||
                this->_players[number].y() - (double)iy > 0.5))
            {
                irot = 22;
                break;
            }
            else if ((this->walkable(ix + 1, iy) || this->_players[number].x() - (double)ix < 0.5) &&
                !this->walkable(ix, iy - 1))
            {
                irot = 0;
                break;
            }
            else if (this->walkable(ix + 1, iy) && this->walkable(ix, iy - 1))
            {
                if (this->_players[number].current_axis() == Player::HORIZONTAL)
                {
                    aux = this->_players[number].x() - (double)ix;
                    this->_players[number].move(fmul * fmul + smul * smul);
                    if (aux < 0.5 && this->_players[number].x() - (double)ix >= 0.5)
                    {
                        this->_players[number].current_axis(Player::VERTICAL);
                        this->_players[number].x((double)ix + 0.5);
                        this->_players[number].y((double)iy + 0.495);
                    }
                }
                else
                {
                    aux = this->_players[number].y() - (double)iy;
                    this->_players[number].move(-(fmul * fmul + smul * smul));
                    if (aux > 0.5 && this->_players[number].y() - (double)iy <= 0.5)
                    {
                        this->_players[number].current_axis(Player::HORIZONTAL);
                        this->_players[number].y((double)iy + 0.5);
                        this->_players[number].x((double)ix + 0.505);
                    }
                }
            }
            repeat = false;
            break;


        }
    while (repeat);


    //if (this->_level[ix + iy * this->_width
    //this->_players[number].move(mul);

}
