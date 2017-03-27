/*
author: dou
*/

#pragma once
#include "OpenGL.h"
#include <vector>
#include <string>

class Texture2D
{
public:
	static int getPoT(int v);

	static int getPixelSize(int format);

	Texture2D();

	~Texture2D();

	bool Load(std::string path);

	bool LoadAlpha(GLubyte* buffer, GLuint width, GLuint height);

	bool Load(GLubyte* buffer, GLuint width, GLuint height, int format);

	void LoadAlias(GLuint id, GLuint width, GLuint height, int format);

	void Free();

	void Update(GLubyte* buffer, Rect2Di* clip);

	virtual void AddQuad(GLfloat x, GLfloat y, Rect2Di* clip = NULL, Color4f* color = NULL);

	void Draw();

	void Clear();

	GLuint getID();

	GLuint getWidth();

	GLuint getHeight();

protected:
	//Texture name 
	GLuint mTextureID;

	//Pixel format 
	GLuint mPixelFormat;

	GLuint mImageWidth;
	GLuint mImageHeight;

	//Texture dimensions 
	GLuint mTextureWidth;
	GLuint mTextureHeight;

	int DrawCount;
	std::vector<VertexData2D> vBuffer;
	std::vector<int> iBuffer;
};