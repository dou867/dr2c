/*
author: dou
*/

#include "Font.h"
#include "Global.h"

FT_Library Font::library;

Font::Font()
{
	count = 0;
	texw = 2048;
	texh = 2048;
	pixsize = 4;

	cellbuf = new GLubyte[cellw * cellh * pixsize];

	int maxchars = texw / cellw * texh / cellh;
	gData = new GlyphData[maxchars];
	memset(gData, 0, sizeof(gData) * maxchars);
	maxBearing = 0;
	maxHang = 0;

	lineHeight = 0;
}

Font::~Font()
{
	FT_Done_Face(face);
	delete cellbuf;
	delete gData;
}

bool Font::init()
{
	FT_Init_FreeType(&library);
	return true;
}

void Font::load(string file, int size)
{
	this->path = file;
	this->size = size;

	GLubyte *texbuf = new GLubyte[texw * texh * pixsize];
	memset(texbuf, 0, texw * texh * pixsize);

	tex.Load(texbuf, texw, texh, GL_RGBA);
	delete texbuf;

	FT_New_Face(library, path.c_str(), 0, &face);
	FT_Set_Pixel_Sizes(face, 0, size);

	for (int i = 0x20; i < 0x80; i++)
		addGlyph(i);
}

void Font::LoadAlias(GLuint id, GLuint width, GLuint height, int format)
{
	alias.LoadAlias(id, width, height, format);
	FGalias.LoadAlias(id, width, height, format);
}

void Font::AddText(GLfloat x, GLfloat y, const wchar_t* text, int len, int alignment)
{
	AddText(x, y, text, len, alignment, alias);
}

void Font::AddFGText(GLfloat x, GLfloat y, const wchar_t* text, int len, int alignment)
{
	AddText(x, y, text, len, alignment, FGalias);
}

void Font::AddText(GLfloat x, GLfloat y, const wchar_t* text, int len, int alignment, Alias& alias)
{
	int c = count;

	int linelen = 0;

	int(*cf)(unsigned char, int, int) = (int(*)(unsigned char, int, int))(*glyphcb);

	for (int i = 0; i < len; i++)
	{
		wchar_t c = text[i];

		if (c == 0)
			continue;

		if (!gIndex.count(c))
		{
			if (c < 0x100)
			{
				if (!cf || !cf(c, 0, *glyphcbc))
				{
					TexInfo* tex = TexInfoTable + c;
					linelen += tex->width * gquad->wscale;
				}
			}
			else
			{
				addGlyph(c);
			}
		}
		if (gIndex.count(c))
		{
			linelen += gData[gIndex[c]].advance * gquad->wscale / 4;
		}
	}

	if (alignment == 1)
		x -= linelen / 2;
	if (alignment == 2)
		x -= linelen;

	for (int i = 0; i < len; i++)
	{
		wchar_t c = text[i];

		if (c == 0)
			continue;

		if (!gIndex.count(c))
		{
			//if (0xf0<= c && cf)
			//	cf(c, 0, *glyphcbc);
			if (!cf || !cf(c, 0, *glyphcbc))
			{
				TexInfo* tex = TexInfoTable + c;
				Rect2Di clip{ tex->xpos, tex->ypos, tex->width , tex->height };
				Color4f color{ gquad->r, gquad->g, gquad->b, gquad->a };
				alias.AddQuad(x + tex->xoffset, y + tex->yoffset - tex->height * gquad->hscale / 2, &clip, &color);
				x += tex->width * gquad->wscale;
			}
			else
			{
				if(cf)
					cf(c, 1, *glyphcbc);
			}
		}
		else
		{
			GlyphData &data = gData[gIndex[c]];

			Rect2Di clip{ data.x, data.y, data.width, data.height };
			Color4f color{ gquad->r, gquad->g, gquad->b, gquad->a };
			tex.AddQuad(x + data.bearingx * gquad->wscale / 4, y + (maxBearing - data.bearingy - lineHeight / 2)  * gquad->hscale / 4, &clip, &color);
			x += data.advance * gquad->wscale / 4;
		}
	}
}

void Font::Draw()
{
	tex.Draw();
	alias.Draw();
}

void Font::Clear()
{
	tex.Clear();
	alias.Clear();
}

void Font::FGDraw()
{
	FGalias.Draw();
}

void Font::FGClear()
{
	FGalias.Clear();
}

bool Font::addGlyph(int c)
{
	FT_Error error = FT_Load_Char(face, c, FT_LOAD_RENDER);
	if (!error)
	{
		FT_Bitmap &bitmap = face->glyph->bitmap;
		FT_Glyph_Metrics &metric = face->glyph->metrics;
		int cpr = texw / cellw;
		int posx = (count % cpr) * cellw;
		int posy = count / cpr * cellh;

		memset(cellbuf, 0, sizeof(cellbuf));

		for (int y = 0; y < bitmap.rows; y++)
			for (int x = 0; x < bitmap.width; x++)
			{
				cellbuf[(y * cellw + x) * pixsize] = 0xff;
				cellbuf[(y * cellw + x) * pixsize + 1] = 0xff;
				cellbuf[(y * cellw + x) * pixsize + 2] = 0xff;
				cellbuf[(y * cellw + x) * pixsize + 3] = bitmap.buffer[y * bitmap.width + x];
			}

		Rect2Di clip = { posx,posy,cellw,cellh };
		tex.Update(cellbuf, &clip);

		gData[count].x = posx;
		gData[count].y = posy;
		gData[count].width = bitmap.width;
		gData[count].height = bitmap.rows;
		gData[count].bearingx = metric.horiBearingX >> 6;
		gData[count].bearingy = metric.horiBearingY >> 6;
		gData[count].advance = metric.horiAdvance >> 6;

		if (maxBearing < gData[count].bearingy)
			maxBearing = gData[count].bearingy;
		if (maxHang < gData[count].height - gData[count].bearingy)
			maxHang = gData[count].height - gData[count].bearingy;
		if (lineHeight < gData[count].height)
			lineHeight = gData[count].height;

		gIndex[c] = count;

		count++;
	}
	return !error;
}