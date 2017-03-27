/*
author: dou
*/

#pragma once

#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <iostream>

struct VertexCoord2D
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct TexCoord2D
{
	GLshort u;
	GLshort v;
};

struct Color4b
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct Color4f
{
	float r;
	float g;
	float b;
	float a;
};

struct VertexData2D
{
	VertexCoord2D vertexCoord;
	TexCoord2D texCoord;
	Color4b color;
	int pad[3];
};

struct Rect2Df
{
	float x;
	float y;
	float w;
	float h;
};

struct Rect2Di
{
	int x;
	int y;
	int w;
	int h;
};