/*
author: dou
*/

#include "Texture2D.h"
#include "Global.h"
#include <IL\il.h>
#include <IL\ilu.h>

int Texture2D::getPoT(int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

int Texture2D::getPixelSize(int format)
{
	switch (format)
	{
	case GL_ALPHA:
		return 1;
	case GL_RGBA:
		return 4;
	default:
		break;
	}
	return 0;
}

Texture2D::Texture2D()
{
	mTextureID = 0;

	mPixelFormat = 0;

	mImageWidth = 0;
	mImageHeight = 0;

	mTextureWidth = 0;
	mTextureHeight = 0;

	DrawCount = 0;
}

Texture2D::~Texture2D()
{
	Free();
}

bool Texture2D::Load(std::string path)
{
	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	bool success = ilLoadImage(path.c_str());

	if (success == IL_TRUE)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE)
		{
			success = Load((GLubyte*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT), GL_RGBA);
		}
	}

	ilDeleteImages(1, &imgID);

	return success;
}

bool Texture2D::LoadAlpha(GLubyte* buffer, GLuint width, GLuint height)
{
	int size = width * height;
	GLubyte* buffer32 = new GLubyte[size * 4];
	memset(buffer32, 0, size * 4);
	for (int i = 0; i < size; i++)
	{
		buffer32[i * 4] = 0xff;
		buffer32[i * 4 + 1] = 0xff;
		buffer32[i * 4 + 2] = 0xff;
		buffer32[i * 4 + 3] = buffer[i];
	}
	bool r = Load(buffer32, width, height, GL_RGBA);
	delete buffer32;
	return r;
}

bool Texture2D::Load(GLubyte* buffer, GLuint width, GLuint height, int format)
{
	Free();

	mImageWidth = width;
	mImageHeight = height;

	mTextureWidth = getPoT(mImageWidth);
	mTextureHeight = getPoT(mImageHeight);

	if (mTextureWidth < mTextureHeight)
		mTextureWidth = mTextureHeight;
	if (mTextureHeight < mTextureWidth)
		mTextureHeight = mTextureWidth;


	GLubyte* pad = buffer;

	//pad texture
	if (mImageWidth != mTextureWidth || mImageHeight != mTextureHeight)
	{
		int pixelSize = getPixelSize(format);
		int bufferSize = mTextureWidth * mTextureHeight * pixelSize;
		pad = new GLubyte[bufferSize];
		memset(pad, 0, bufferSize);
		for (int i = 0; i < mImageHeight; i++)
			memcpy(pad + mTextureWidth * pixelSize * i, buffer + mImageWidth * pixelSize * i, mImageWidth * pixelSize);
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, mTextureWidth, mTextureHeight, 0, format, GL_UNSIGNED_BYTE, pad);

	//Set texture parameters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(GL_TEXTURE_2D, NULL);

	if (pad != buffer)
		delete pad;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Free();
		return false;
	}

	mPixelFormat = format;

	return true;
}

void Texture2D::LoadAlias(GLuint id, GLuint width, GLuint height, int format)
{
	mTextureID = id;
	mImageWidth = width;
	mImageHeight = height;
	mTextureWidth = width;
	mTextureHeight = height;
	mPixelFormat = format;
}

void Texture2D::AddQuad(GLfloat x, GLfloat y, Rect2Di* clip, Color4f* color)
{
	//If the texture exists 
	if (mTextureID == 0)
		return;

	//Texture coordinates 
	GLshort texTop = 0;
	GLshort texBottom = mImageHeight;
	GLshort texLeft = 0;
	GLshort texRight = mImageWidth;

	//Vertex coordinates 
	GLfloat quadWidth = mImageWidth * gquad->wscale / 4;
	GLfloat quadHeight = mImageHeight * gquad->hscale / 4;

	//Handle clipping
	if (clip)
	{
		//Texture coordinates
		texLeft = clip->x;
		texRight = clip->x + clip->w;
		texTop = clip->y;
		texBottom = clip->y + clip->h;

		//Vertex coordinates
		quadWidth = clip->w * gquad->wscale / 4;
		quadHeight = clip->h * gquad->hscale / 4;
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

void Texture2D::Draw()
{
	if (DrawCount == 0)
		return;

	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(VertexData2D), &(vBuffer[0].vertexCoord));
	glTexCoordPointer(2, GL_SHORT, sizeof(VertexData2D), &(vBuffer[0].texCoord));
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(VertexData2D), &(vBuffer[0].color));

	glDrawElements(GL_QUADS, DrawCount, GL_UNSIGNED_INT, iBuffer.data());

	//glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Texture2D::Clear()
{
	DrawCount = 0;
}

void Texture2D::Update(GLubyte* buffer, Rect2Di* clip)
{
	if (mTextureID == 0)
		return;
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	Rect2Di texclip = { 0, 0, mTextureWidth, mTextureHeight };

	if (!clip)
		clip = &texclip;

	glTexSubImage2D(GL_TEXTURE_2D, 0, clip->x, clip->y, clip->w, clip->h, mPixelFormat, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Texture2D::Free()
{
	if (mTextureID != 0)
	{
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}

	mImageWidth = 0;
	mImageHeight = 0;

	mTextureWidth = 0;
	mTextureHeight = 0;

	mPixelFormat = NULL;

	DrawCount = 0;
}

GLuint Texture2D::getID()
{
	return mTextureID;
}

GLuint Texture2D::getWidth()
{
	return mImageWidth;
}

GLuint Texture2D::getHeight()
{
	return mImageHeight;
}