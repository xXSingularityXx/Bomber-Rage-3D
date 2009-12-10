#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "tile.hpp"
#include "enemy.hpp"

void Tile::load_tiles(const char *filename, const std::vector<GLuint> &textures, Tile::Tile *tiles[256])
{
    unsigned char id, command, attribute;
    unsigned long int index;
    std::fstream tiles_file;
    GLfloat x, y, z;

    tiles_file.open(filename, std::fstream::in);
    while (true)
    {
        do
            tiles_file >> command;
        while (!tiles_file.eof() && command != '{');

        if (tiles_file.eof())
            break;
        tiles_file >> id;

        do
        {
            tiles_file >> command;
            switch (command)
            {
            case 'd': // Start Display List
                tiles[id] = new Tile::Tile;
                tiles[id]->_display_list = glGenLists(1);
                glNewList(tiles[id]->_display_list, GL_COMPILE);
                break;
            case 'c': // Texture
                tiles_file >> index;
                glBindTexture(GL_TEXTURE_2D, textures[index]);
                break;
            case 'n': // glBegin
                tiles_file >> command;
                switch (command)
                {
                case 'q':
                    glBegin(GL_QUADS);
                    break;
                }
                break;
            case 't': // Texture vertex
                tiles_file >> x >> y;
                glTexCoord2d(x, y);
                break;
            case 'v': // Vertex
                tiles_file >> x >> y >> z;
                glVertex3f(x, y, z);
                break;
            case 'r': // Normal
                tiles_file >> x >> y >> z;
                glNormal3f(x, y, z);
            case 'm': // glEnd
                glEnd();
                break;
            case 'e': // Display list end
                glEndList();
                break;
            case 'p': // Probabilistic tile
                tiles[id] = new Tile::Tile;
                tiles_file >> tiles[id]->_probabilities_size;
                tiles[id]->_probabilities = new double[tiles[id]->_probabilities_size - 1];
                tiles[id]->_clones = new unsigned char[tiles[id]->_probabilities_size];

                for (unsigned long i = 0; i < tiles[id]->_probabilities_size - 1; i++)
                    tiles_file >> tiles[id]->_probabilities[i];
                srand(time(NULL));
                break;
            case 'o': // Clone tile
                tiles_file >> index;
                tiles_file >> tiles[id]->_clones[index];
                break;
            case 'a': // Attribute
                tiles_file >> attribute;
                switch (attribute)
                {
                    case 'w': // Walkable
                        tiles[id]->_walkable = true;
                        break;
                    case 'b': // Breakable
                        tiles[id]->_breakable = true;
                        break;
                }
                break;
            case 'y': // Enemy
                tiles[id]->_enemy = true;
                break;
            }
        } while (command != '}');
    }
    tiles_file.close();

    return;
}

Tile::Tile(): _display_list(0), _probabilities_size(0), _probabilities(NULL), _clones(NULL), _walkable(false), _breakable(false), _enemy(false)
{ }

Tile::~Tile()
{
    if (this->_probabilities != NULL)
        delete [] this->_probabilities;
    if (this->_clones != NULL)
        delete [] this->_clones;
}

unsigned char Tile::draw(bool &new_enemy)
{
    if (this->_enemy)
    {
        new_enemy = true;
        this->_enemy = false;
    }
    else
        new_enemy = false;

    if (this->_probabilities_size == 0)
        glCallList(this->_display_list);
    else
    {
        unsigned long i;
        double probability = (double)rand() / RAND_MAX;

        for (i = 0; i < this->_probabilities_size - 1; i++)
            if ((probability -= this->_probabilities[i]) < 0.0)
                break;
        return this->_clones[i];
    }
    return 0;
}
