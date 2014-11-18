#ifndef GL_HEADERS_H
#define GL_HEADERS_H

#define GLEW_STATIC
#include <GL/glew.h>

#ifdef __APPLE__
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_opengl.h>
#else
	#include <SDL.h>
	#include <SDL_opengl.h>
#endif

#endif