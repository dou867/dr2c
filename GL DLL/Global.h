/*
author: dou
*/

#pragma once
#include "OpenGL.h"

struct TexInfo
{
	short xpos;
	short ypos;
	short width;
	short height;
	int unkown[3];
	short xoffset;
	short yoffset;
	int pTexData;
};

struct RenderInfo
{
	int unkown;
	int texw;
	int texh;
	int texid;
	int unkown2[4];
	void* pvertexbuffer;
	int numelements;
	int buffersize;
	int unkown3[2];
	int pvertexbuffer2;
	int numelements2;
	int buffersize2;
	int unkown4[2];
};

struct QuadInfo
{
	double x;
	double y;
	double z;
	double wscale;
	double hscale;
	double dscale;
	double ywscale;
	double xwscale;
	double yhscale;
	double xhscale;
	float r;
	float g;
	float b;
	float a;
};

extern int* glyphcb;
extern int* glyphcbc;

extern TexInfo* TexInfoTable;
extern RenderInfo* renderer;
extern QuadInfo* gquad;