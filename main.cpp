#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

#include <GL/glu.h>

using namespace std;

const char* vertexShaderSrc = "#version 150\nin vec2 position;\nvoid main()\n{\ngl_Position = vec4(position, 0.0, 1.0);\n}\n";
const char* fragmentShaderSrc = "#version 150\nout vec4 outColor;\nvoid main()\n{\noutColor = vec4(1.0,1.0,1.0,1.0);\n}\n";

int main()
{
    SDL_Window* win;
    SDL_GLContext context;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        cout<<"Error with initing SDL.\n";
        return 1;
    }

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    /*SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);*/

    win = SDL_CreateWindow("Boids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(win);

    glewExperimental = GL_FALSE;
    if(GLEW_OK != glewInit())
    {
        cout<<"Error initing glew.\n";
        return 0;
    }

    //SDL_GL_MakeCurrent(win, context);

    //SDL_GL_SetSwapInterval(1);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    printf("%u\n",vertexBuffer);

    float verts[] =
    {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    //Generate vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    //Check the shader
    {
        GLint status;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
        if(status != GL_TRUE)
        {
            char buffer[512];
            glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
            cout<<buffer<<"\n";
        }
    }

    //Generate fragment shader
    GLuint fragmentShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    //Check the shader
    {
        GLint status;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
        if(status != GL_TRUE)
        {
            char buffer[512];
            glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
            cout<<buffer<<"\n";
        }
    }

    //Making a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    //Something is wrong here...

    GLenum e = glGetError() ;
    if(e != GL_NO_ERROR)
    {
        printf("Error with openGL: %s\n",gluErrorString(e));
        return 3;
    }

    GLint posAttribute = glGetAttribLocation(shaderProgram,"position");
    glVertexAttribPointer(posAttribute,2,GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttribute);

    //vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    SDL_Event windowEvent;

    while(true)
    {
        if(SDL_PollEvent(&windowEvent))
        {
            if(windowEvent.type == SDL_QUIT) break;
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(win);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
