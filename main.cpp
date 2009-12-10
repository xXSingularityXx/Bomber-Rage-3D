/*
 * Copyright notice (not there yet, as you may have noticed)
 */


#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "player.hpp"
#include "level.hpp"
#include "bomb.hpp"
/*
#ifndef M_PI
#define M_PI 3.14159265
#endif
*/

#define FRAMESKIPPING 0
#define PLAYER_HEIGHT 0.65

static GLint T0 = 0;
static GLint Frames = 0;
SDL_Surface *screen;
static Level current_level(1);

void draw(bool render)
{
    if (render)
    {
        static GLfloat light_pos[4] = {-1.0f, 1.0f, 1.0f, 0.0f};

        //static GLfloat green[4] = {0.1, 0.8, 0.2, 1.0};
        //glClearColor(0.7f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        gluLookAt(current_level.player(0).x(), PLAYER_HEIGHT, current_level.player(0).y(),
            current_level.player(0).x() + cos(current_level.player(0).rot()),
            sin(current_level.player(0).vrot()) + PLAYER_HEIGHT,
            current_level.player(0).y() + sin(current_level.player(0).rot()),
            0.0, 1.0, 0.0);

        //gluLookAt(3.0, 3.0, 3.0, current_level.player(0).x(), PLAYER_HEIGHT, current_level.player(0).y(), 0.0, 1.0, 0.0);

        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        current_level.draw();
        glPopMatrix();

        SDL_GL_SwapBuffers();
    }

    Frames++;
    {
        GLint t = SDL_GetTicks();
        if (t - T0 >= 1000)
        {
            char caption[1000];

            GLfloat seconds = (t - T0) / 1000.0;
            GLfloat fps = Frames / seconds;
            sprintf(caption, "Bomber Rage 3D - %d frames in %g seconds = %g FPS", Frames, seconds, fps);
            SDL_WM_SetCaption(caption, "bomberman");
            T0 = t;
            Frames = 0;
        }
    }
}

void update()
{
    current_level.update();
}

void init(int argc, char *argv[])
{
    //static GLfloat light_pos[4] = {5.0, 3.0, 9.0, 0.0};
    GLfloat density = 0.07; //set the density
    GLfloat fog_color[4] = {0.85, 0.85, 1.0, 1.0}; //set the fog color

        //glEnable(GL_LINE_SMOOTH);

    glEnable(GL_TEXTURE_2D);

    //glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST); //enable the depth testing
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_COLOR_MATERIAL);

    GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screen->w / (GLfloat)screen->h, 0.01f, 500.0f);
	//gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glLoadIdentity();
    //glScalef((double)screen->h / screen->w, 1.0, 1.0);
    //glEnable(GL_NORMALIZE);

    glEnable(GL_FOG); //enable the fog
    glFogi(GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2

    glFogfv(GL_FOG_COLOR, fog_color); //set the fog color to our color chosen above
    glFogf(GL_FOG_DENSITY, density); //set the density to the value above
        glHint(GL_FOG_HINT, GL_NICEST); // set the fog to look the nicest, may slow down on older cards

        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

}

bool done = false;

void process_keys()
{
    Uint8 *keys;
    double fmul = 0.0, smul = 0.0;

    keys = SDL_GetKeyState(NULL);

    if (keys[SDLK_ESCAPE])
        done = true;
    if (keys[SDLK_w])
        fmul += 1.0;
    if (keys[SDLK_s])
        fmul -= 1.0;
    if (keys[SDLK_a])
        smul -= 1.0;
    if (keys[SDLK_d])
        smul += 1.0;
    if (keys[SDLK_SPACE])
    {
        bool allow = current_level.player(0).dropped_bombs() < current_level.player(0).max_bombs();
        if (allow)
        {
            unsigned long x = (unsigned long)current_level.player(0).x();
            unsigned long y = (unsigned long)current_level.player(0).y();
            for (std::list<Object *>::iterator it = current_level.objects(x, y).begin();
                it != current_level.objects(x, y).end(); it++)
                if ((*it)->unique())
                {
                    allow = false;
                    break;
                }
            if (allow)
            {
                current_level.player(0).dropped_bombs(current_level.player(0).dropped_bombs() + 1);
                current_level.new_object(new Bomb(current_level.player(0).x(), current_level.player(0).y(), current_level.player(0).explosion_size(), current_level, 0), x, y);
            }
        }
    }

    if (fmul != 0.0 && smul != 0.0)
    {
        fmul *= 0.70710678;
        smul *= 0.70710678;
    }
    current_level.move_player(0, fmul, smul);
}

int main(int argc, char *argv[])
{
    GLint ticks = 0;
    int fs = 0;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    screen = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL | SDL_FULLSCREEN);
    if (!screen)
    {
        fprintf(stderr, "Couldn't set GL video mode: %s\n", SDL_GetError());
        SDL_Quit();
        exit(2);
    }
    SDL_WM_SetCaption("Bomber Rage 3D", "brage3d");
    SDL_ShowCursor(SDL_DISABLE);

    init(argc, argv);
    //reshape(screen->w, screen->h);

    current_level.load("data/scenario/default/level1.txt", 0);
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_MOUSEMOTION:
                    current_level.player(0).rotate((double)event.motion.xrel * 0.006);
                    current_level.player(0).vertical_rotate((double)event.motion.yrel * 0.006);
                    //SDL_WarpMouse(screen->w / 2, screen->h / 2);
                    break;
            }
        }

        process_keys();
        update();
        draw(fs == 0);
        if (++fs > FRAMESKIPPING)
            fs = 0;

        GLint t = SDL_GetTicks();
        while (t - ticks < 12)
        {
            Sleep(5);
            t = SDL_GetTicks();
        }
        ticks = t;
    }
    SDL_Quit();
    return 0;
}
