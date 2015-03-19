cmake_minimum_required(VERSION 2.6)

project(Boids)

set(HDRS
	./include/Boid.h
	./include/Client.h
	./include/Flock.h
	./include/NetworkInterface.h
	./include/Server.h
)

set(SRCS
	./main.cpp
	./src/Boid.cpp
	./src/Client.cpp
	./src/Flock.cpp
	./src/NetworkInterface.cpp
	./src/Server.cpp
)

find_path(RakNet_INCLUDE_PATH REQUIRED)
find_path(RakNet_DEBUG_LIB_PATH REQUIRED)
find_path(RakNet_RELEASE_LIB_PATH REQUIRED)

find_path(GLEW_INCLUDE_PATH REQUIRED)
find_path(GLEW_DEBUG_LIB_PATH REQUIRED)
find_path(GLEW_RELEASE_LIB_PATH REQUIRED)

find_path(SDL_INCLUDE_PATH REQUIRED)
find_path(SDL_DEBUG_LIB_PATH REQUIRED)
find_path(SDL_RELEASE_LIB_PATH REQUIRED)

find_path(GLM_INCLUDE_PATH REQUIRED)

include_directories(${RakNet_INCLUDE_PATH} ${GLEW_INCLUDE_PATH} ${SDL_INCLUDE_PATH} ${GLM_INCLUDE_PATH} "./include")

add_executable(Boids ${HDRS} ${SRCS})

target_link_libraries(Boids 
debug ${RakNet_DEBUG_LIB_PATH}/RakNetLibStatic.lib optimized ${RakNet_RELEASE_LIB_PATH_LIB_PATH}/RakNetLibStatic.lib
debug ${GLEW_DEBUG_LIB_PATH}/glew32s.lib optimized ${GLEW_RELEASE_LIB_PATH}/glew32s.lib
debug ${SDL_DEBUG_LIB_PATH}/SDL2.lib optimized ${SDL_RELEASE_LIB_PATH}/SDL2.lib
OpenGL32.lib
ws2_32.lib
)