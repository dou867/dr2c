/*
author: dou
*/

#include "alias.h"
#include "Global.h"

void Alias::AddQuad(GLfloat x, GLfloat y, Rect2Di* clip, Color4f* color)
{
	if (mTextureID == 0)
		return;

	//Texture coordinates 
	GLshort texTop = mImageHeight;
	GLshort texBottom = 0;
	GLshort texLeft = 0;
	GLshort texRight = mImageWidth;

	//Vertex coordinates 
	GLfloat quadWidth = mImageWidth * gquad->wscale;
	GLfloat quadHeight = mImageHeight * gquad->hscale;

	//Handle clipping
	if (clip)
	{
		//Texture coordinates
		texTop = mImageHeight - clip->y;
		texBottom = mImageHeight - clip->y - clip->h;
		texLeft = clip->x;
		texRight = clip->x + clip->w;

		//Vertex coordinates
		quadWidth = clip->w * gquad->wscale;
		quadHeight = clip->h * gquad->hscale;
	}

	GLubyte r = 0xff;
	GLubyte g = 0xff;
	GLubyte b = 0xff;
	GLubyte a = 0xff;

	if (color)
	{
		r = r * color->r;
		g = g * color->g;
		b = b * color->b;
		a = a * color->a;
	}

	if (vBuffer.size() < DrawCount + 4)
	{
		VertexData2D d;
		vBuffer.push_back(d);
		vBuffer.push_back(d);
		vBuffer.push_back(d);
		vBuffer.push_back(d);
	}

	//Texture coordinates
	vBuffer[DrawCount].texCoord = { texLeft, texTop };
	vBuffer[DrawCount + 1].texCoord = { texRight, texTop };
	vBuffer[DrawCount + 2].texCoord = { texRight, texBottom };
	vBuffer[DrawCount + 3].texCoord = { texLeft, texBottom };

	//Vertex positions 
	vBuffer[DrawCount].vertexCoord = { x, y, 0 };
	vBuffer[DrawCount + 1].vertexCoord = { x + quadWidth, y, 0 };
	vBuffer[DrawCount + 2].vertexCoord = { x + quadWidth, y + quadHeight, 0 };
	vBuffer[DrawCount + 3].vertexCoord = { x, y + quadHeight, 0 };

	vBuffer[DrawCount].color = { r,g,b,a };
	vBuffer[DrawCount + 1].color = { r,g,b,a };
	vBuffer[DrawCount + 2].color = { r,g,b,a };
	vBuffer[DrawCount + 3].color = { r,g,b,a };

	DrawCount += 4;

	int is = iBuffer.size();
	if (is < DrawCount)
	{
		iBuffer.push_back(is);
		iBuffer.push_back(is + 1);
		iBuffer.push_back(is + 2);
		iBuffer.push_back(is + 3);
	}
}