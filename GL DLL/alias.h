/*
author: dou
*/

#pragma once
#include "Texture2D.h"

class Alias : public Texture2D
{
public:
	void Alias::AddQuad(GLfloat x, GLfloat y, Rect2Di* clip = NULL, Color4f* color = NULL) override;
};