/*
author: dou
*/

#pragma once
#include <ft2build.h>
#include "Texture2D.h"
#include "alias.h"
#include <map>
#include <memory>
#include FT_FREETYPE_H

using namespace std;

struct GlyphData
{
	int x;
	int y;
	int width;
	int height;
	int bearingx;
	int bearingy;
	int advance;
};

class Font
{
public:
	static bool init();

	Font();

	~Font();

	void load(string file, int size);

	void LoadAlias(GLuint id, GLuint width, GLuint height, int format);

	void AddText(GLfloat x, GLfloat y, const wchar_t* text, int len, int alignment);

	void AddFGText(GLfloat x, GLfloat y, const wchar_t* text, int len, int alignment);

	bool addGlyph(int c);

	void Draw();

	void Clear();

	void FGDraw();

	void FGClear();

	int getSize();

private:
	static FT_Library library;
	std::string path;
	int size;
	FT_Face face;
	std::map<int, int> gIndex;
	GlyphData* gData;
	Texture2D tex;
	Alias alias;
	Alias FGalias;
	GLubyte* cellbuf;
	int count;
	int texw;
	int texh;
	int pixsize;
	const int cellw = 32;
	const int cellh = 32;

	int maxBearing;
	int maxHang;
	int lineHeight;

	void AddText(GLfloat x, GLfloat y, const wchar_t* text, int len, int alignment, Alias& alias);
};