// Link statically with GLEW
#define GLEW_STATIC

// Headers
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#undef main
#include <time.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "NetworkInterface.h"
#include "Server.h"
#include "Client.h"

#include "Flock.h"
#include "Boid.h"

float expectedTimeBetweenUpdates = 5.0f;

// Shader sources
const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec3 position;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 proj;"
    "void main() {"
    "   gl_Position = proj * view * model * vec4(position, 1.0);"
    "}";
const GLchar* fragmentSource =
	"#version 150 core\n"
	"uniform vec3 colour;"
    "out vec4 outColor;"
    "void main() {"
    "   outColor = vec4(colour, 1.0);"
    "}";
using namespace std;
int main()
{
    srand(time(NULL));

    SDL_Window* win;
    SDL_GLContext context;
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
    cout<<"Error with initing SDL.\n";
    return 1;
    }
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    /*SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);*/
    win = SDL_CreateWindow("Boids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(win);

    SDL_GL_MakeCurrent(win, context);
    SDL_GL_SetSwapInterval(1);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Open up the obj file
    std::fstream file;
    file.open("Boid.obj", std::fstream::in);

    std::vector<GLfloat> vertices;
    std::vector<unsigned int> index;

    char buffer[128];
    while(!file.eof())
    {
        file.getline(buffer, 128);

        if(buffer[0] == 'v')
        {
            float x,y,z;

            sscanf(buffer, "%*[^ ]%f %f %f", &x, &y, &z);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else if(buffer[0] == 'f')
        {
            int f1, f2, f3;

            sscanf(buffer, "%*[^ ]%i%*[^ ]%i%*[^ ]%i", &f1, &f2, &f3);

            index.push_back(f1-1);
            index.push_back(f2-1);
            index.push_back(f3-1);
        }
    }

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    //IBO
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), &index[0], GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLint uModel = glGetUniformLocation(shaderProgram, "model");
    GLint uView = glGetUniformLocation(shaderProgram, "view");
    GLint uProj = glGetUniformLocation(shaderProgram, "proj");

	GLint uColour = glGetUniformLocation(shaderProgram, "colour");

    glm::mat4 view = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 proj = glm::perspective(glm::radians(60.0f), 1280.0f/720.0f, 0.01f, 10000.0f);
	
    //Set up the network stuff
    NetworkInterface* net;
    cout<<"Should this computer act as the Server or Client?\n 1 for server, 2 for client.\n";

    int selection;
    while(true)
    {
        cin>>selection;
        if(selection == 1)
        {
            net = new Server();
            break;
        }
        else if(selection == 2)
        {
            net = new Client();
            break;
        }
    }

    string ip = "localhost";
    if(selection == 2)
    {
        cout<<"please enter the ip of the server.\n";
        cin>>ip;
    }

    net->Activate(ip.c_str());

    Flock* myFlock = new Flock();

	Boid* remoteFlock[BOID_COUNT];

	for (unsigned int i = 0; i < BOID_COUNT; ++i)
	{
		remoteFlock[i] = new Boid();
	}

	float remoteColour[3] = { 1, 0, 0 };
	float localColour[3] = { 0, 1, 0};

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(proj));

	clock_t startTime = clock();
	double timeSinceLastUpdateSent = 0.0, deltaTime = 0.0f;

    SDL_Event event;
    while (true)
    {
		deltaTime = double(clock() - startTime)/CLOCKS_PER_SEC;
		startTime = clock();
		timeSinceLastUpdateSent += deltaTime;

        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                break;
            }

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_q:
					expectedTimeBetweenUpdates -= 0.1f;
					std::cout << "Time between network updates: " << expectedTimeBetweenUpdates << "\n";
					char b[32];
					sprintf_s(b, "delay %f",expectedTimeBetweenUpdates);
					net->Send(b);
					break;
				case SDLK_w:
					expectedTimeBetweenUpdates += 0.1f;
					std::cout << "Time between network updates: " << expectedTimeBetweenUpdates << "\n";
					char bu[32];
					sprintf_s(bu, "delay %f", expectedTimeBetweenUpdates);
					net->Send(bu);
					break;
				}
			}
        }

        glClear(GL_COLOR_BUFFER_BIT);

        //Get the position of the mouse
        int x, y;
        SDL_GetMouseState(&x, &y);

        float relX, relY;
        relX = (float(x)/(640.0f)) - 1.0f;
        relY = ((float(y)/(360.0f)) - 1.0f) * -1.0f;

        //Update the positions of the remote flock
        net->Recieve(remoteFlock);

        glm::vec3 target = glm::vec3(relX * 110.0f, relY * 70.0f,-100.0f);
        myFlock->Update(target);

		for (unsigned int i = 0; i < BOID_COUNT; ++i)
        {
            //Draw the remote flock
			if (remoteFlock[i]->canDraw)
			{
				//Update the position of the remote boid
				remoteFlock[i]->RemoteUpdate(float(deltaTime));

				glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(remoteFlock[i]->GetTransformation()));
				glUniform3fv(uColour, 1, remoteColour);
				glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
			}

			//Draw the local flock
            glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(myFlock->members[i]->GetTransformation()));
			glUniform3fv(uColour, 1, localColour);
            glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);

			
        }

		//Send the local flock to the other computer
		if (timeSinceLastUpdateSent >= expectedTimeBetweenUpdates)
		{
			for (unsigned int i = 0; i < BOID_COUNT; ++i)
			{
				char buffer[512];
				sprintf(buffer, "%i %f %f %f - %f %f %f", i, myFlock->members[i]->GetTransformation()[3].x, myFlock->members[i]->GetTransformation()[3].y, myFlock->members[i]->GetTransformation()[3].z,
				myFlock->members[i]->GetTransformation()[2][0], myFlock->members[i]->GetTransformation()[2][1], myFlock->members[i]->GetTransformation()[2][2]);

				net->Send(std::string(buffer));
				timeSinceLastUpdateSent = 0.0;
			}
		}

        // Swap buffers
        SDL_GL_SwapWindow(win);
    }

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
