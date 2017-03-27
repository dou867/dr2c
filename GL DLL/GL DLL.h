/*
author: dou
*/

#pragma once

#include <SDL.h>
#include <GL\glew.h>
#include <windows.h>
#include <IL\il.h>
#include <IL\ilu.h>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GLDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GLDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GLDLL_EXPORTS
#define GLDLL_API __declspec(dllexport)
#else
#define GLDLL_API __declspec(dllimport)
#endif

GLDLL_API SDL_GLContext SDLCALL init();

GLDLL_API void AddString(char* buffer);

GLDLL_API void GLAPIENTRY Render(GLenum mode, GLsizei count, GLenum type, const void *indices);

char* warpStr(char **ppstr, int a2, unsigned int maxwidth, int a4);
void AddFGString(char* pBuffer, int alignment);
int strTrs(char* pStr, int a2);
int formatStr(char* buffer, char* format, ...);
int formatStr(char *buf, int length, char *fmt, va_list argv);
char* getLootName(int id);
char* getStatName(unsigned int id);

//GLDLL_API void GLAPIENTRY LoadTex(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);