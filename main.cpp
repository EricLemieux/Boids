#include <iostream>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/gl.h>

using namespace std;

int main()
{
    SDL_Window* win;
    SDL_GLContext context;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        cout<<"Error with initing SDL.\n";
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    win = SDL_CreateWindow("Boids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(win);

    SDL_GL_MakeCurrent(win, context);

    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        cout<<"Error Setting up GLEW.\n";
        return 1;
    }

    cout<<"Renderer: "<<glGetString(GL_RENDER)<<"\n";
    cout<<"Version: "<<glGetString(GL_VERSION)<<"\n";

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    while(true)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(win);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
