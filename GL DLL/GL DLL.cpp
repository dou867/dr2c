/*
author: dou
*/

// GL DLL.cpp : Defines the exported functions for the DLL application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "GL DLL.h"
#include "Font.h"
#include "UTFParser.h"
#include "Global.h"
#include <vector>
#include <map>

TexInfo* TexInfoTable = (TexInfo*)0x555b40;
RenderInfo* renderer = (RenderInfo*)0x4de020;
QuadInfo* gquad = (QuadInfo*)0x4ab060;

int* glyphcb = (int*)0x4f2940;
int* glyphcbc = (int*)0x4f2944;

int renderIndex = 0;

wchar_t wstr[1000];

Texture2D bg;

Font font;
SDL_GLContext context;
Color4f bgColor{ 1,1,1,1 };

GLDLL_API SDL_GLContext SDLCALL init(SDL_Window* window)
{
	context = SDL_GL_CreateContext(window);
	glewInit();

	ilInit();
	iluInit();
	ilClearColour(0, 0, 0, 0);
	struct stat buffer;

	Font::init();
	font.load("font.ttf", 24);

	if (!stat("0.png", &buffer))
	{
		bg.Load("0.png");
		font.LoadAlias(3, 2048, 2048, 0x8034);
		_wrename(L"0.png", L"注意事项.png");
	}
	else
		font.LoadAlias(2, 2048, 2048, 0x8034);

	return context;
}

map<string, char*> m = {
	{ "morale", u8"精神健康" },
	{ "composure", u8"冷静程度" },
	{ "charm", u8"魅力" },
	{ "wits", u8"嘴炮" },
	{ "attitude", u8"个人风格" },
	{ "loyalty", u8"忠诚" },
	{ "strength", u8"力量" },
	{ "fitness", u8"健康" },
	{ "dexterity", u8"敏捷" },
	{ "medical", u8"医学" },
	{ "mechanical", u8"汽修" },
	{ "shooting", u8"射击" },
	{ "vitality", u8"生命值" },

	{ "fuel", u8"燃料"},
	{ "speed", u8"速度" },
	{ "walkin'", u8"行走" },

	{ "MORE... ", u8"更多... " },
	{ "Equip", u8"装备" },
	{ "Status", u8"状态" },
	{ "Vehicle", u8"车辆" },
	{ "\xf5\xfb\xe7\xf0\xfc  START  \xf5\xfb\xe7\xfc", u8"\xf5\xfb\xe7\xf0\xfc  开始  \xf5\xfb\xe7\xfc" },
	{ "INFO", u8"开发人员" },
	{ "QUIT", u8"退出" },
	{ "OPTIONS", u8"设置" },
	{ "Sound", u8"声音" },
	{ "Music", u8"音乐" },
	{ "Go windowed", u8"返回窗口" },
	{ "PLAYER 1 Controls", u8"玩家1控制" },
	{ "PLAYER 2 Controls", u8"玩家2控制" },
	{ "Grain \x0e",u8"纹理 \x0e" },
	{ "Grain \x0f",u8"纹理 \x0f" },
	{ "Glitch \x0e",u8"胶片颗粒 \x0e" },
	{ "Glitch \x0f",u8"胶片颗粒 \x0f" },
	{ "Fast Text \x0e",u8"快速文本 \x0e" },
	{ "Fast Text \x0f",u8"快速文本 \x0f" },
	{ "Scratch \x0e",u8"撕裂效果 \x0f" },
	{ "Scratch \x0f",u8"撕裂效果 \x0f" },
	{ "Fullscreen SOFT",u8"全屏" },
	{ "Fullscreen HARD",u8"无边框" },
	{ "Window size",u8"窗口" },
	{ "Up",u8"上" },
	{ "Down",u8"下" },
	{ "Left",u8"左" },
	{ "Right",u8"右" },
	{ "Attack",u8"攻击" },
	{ "Use",u8"选用" },
	{ "Swap",u8"切换" },
	{ "RESET",u8"默认" },

	{ "Wield", u8"使用中" },
	{ "Carry", u8"备用" },
	{ "Team", u8"小队" },
	{ "Role", u8"角色" },
	{ "ROLE:", u8"角色:" },
	{ "Lead the mission through the scavenging site.", u8"带领搜查任务." },
	{ "Controlled by PLAYER.", u8"玩家控制"},
	{ "Follow and assist the lead member.", u8"跟寻和辅助队长" },
	{ "Rest and recover from afar while guarding team supplies.", u8"在远处休息和保护补给" },
	{ "\x0c LEAD ", u8"\x0c 队长" },
	{ "\x0d LEAD ", u8"\x0d 队长" },
	{ "\x0c FOLLOW ", u8"\x0c 跟随" },
	{ "\x0d FOLLOW ", u8"\x0d 跟随" },
	{ "\x0c REST ", u8"\x0c 休息" },
	{ "\x0d REST ", u8"\x0d 休息" },


	{ "Custom Character", u8"自定角色" },
	{ "HEAD SETTINGS", u8"头部选项" },
	{ "HEAD", u8"头部" },
	{ "BODY", u8"身体" },
	{ "FACE", u8"脸部" },
	{ "HAIR", u8"头发" },
	{ "SKIN CLR", u8"肤色" },
	{ "HAIR CLR", u8"发色" },
	{ "HAT", u8"帽子" },
	{ "SHADES", u8"眼镜" },
	{ "EXTRA", u8"额外部分" },
	{ "Save", u8"保存" },
	{ "BODY SETTINGS", u8"身体选项" },
	{ "GENDER", u8"性别" },
	{ "SIZE", u8"体型" },
	{ "TOP", u8"上装" },
	{ "BOTTOMS", u8"下装" },

	{ "Leader", u8"队长" },
	{ "Buddy", u8"伙伴" },
	{ "NAME", u8"名字" },
	{ "PERK", u8"特技" },
	{ "TRAIT", u8"特性" },
	{ "No buddy", u8"没伙伴" },
	{ "With buddy", u8"有伙伴" },
	{ "Random", u8"随机" },
	{ "Load", u8"读取" },
	{ "Start", u8"开始" },
	{ "Game Mode", u8"游戏模式" },

	{ "\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02nothing", u8"\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02无" },

	{ "Swap Meet", u8"物品交换" },
	{ "\xF1""COMBAT >>> ", u8"攻击模式 >>>" },
	{ "\x0c SMASHIN! ", u8"\x0c 近战！" },
	{ "\x0d SMASHIN! ", u8"\x0d 近战！" },
	{ "\x0c EITHER ", u8"\x0c 电脑控制" },
	{ "\x0d EITHER ", u8"\x0d 电脑控制" },
	{ "\x0c SHOOTIN! ", u8"\x0c 射击" },
	{ "\x0d SHOOTIN! ", u8"\x0d 射击" },
	{ "\xf1""TACTIC >>> ", u8"战术 >>>" },
	{ "\x0c RUNNIN! ", u8"\x0c 跑啊" },
	{ "\x0d RUNNIN! ", u8"\x0d 跑啊" },
	{ "\x0c DEFEND ", u8"\x0c 防守" },
	{ "\x0d DEFEND ", u8"\x0d 防守" },
	{ "\x0c FIGHTIN! ", u8"\x0c 进攻" },
	{ "\x0d FIGHTIN! ", u8"\x0d 进攻" },
};

GLDLL_API void AddString(int a1, char* pBuffer, int strlen, int alignment, int textGlow)
{
	if (renderer->numelements > renderIndex)
		renderIndex = renderer->numelements;
	string s(pBuffer);
	if (m.count(s))
		pBuffer = m[s];
	UTFParser parser;
	const vector<wchar_t> wstr = parser.ParseString(pBuffer);
	font.AddText(gquad->x, gquad->y, wstr.data() , wstr.size(), alignment);
}

void AddFGString(char* pBuffer, int alignment)
{
	UTFParser parser;
	const vector<wchar_t> wstr = parser.ParseString(pBuffer);
	font.AddFGText(gquad->x, gquad->y, wstr.data(), wstr.size(), alignment);
}

char* warpStr(char **ppstr, int a2, unsigned int maxwidth, int multiLineBreak)
{
	if (!ppstr || !*ppstr || !**ppstr)
		return 0;

	UTFParser parser;
	char* out = (char*)0x72d440;
	ZeroMemory(out, 0xFF);
	int totalWidth = 0;
	int constWidth = 6;
	char* s = *ppstr;

	while (*s != 0)
	{
		if (*s == '\r' || *s == '\n' || *s == '\t')
		{
			memcpy(out, *ppstr, s - *ppstr);
			*ppstr = ++s;
			return out;
		}
		s += parser.StrAdv(s);
		totalWidth += constWidth;
		if (totalWidth > maxwidth)
			break;
	}
	memcpy(out, *ppstr, s - *ppstr);
	*ppstr = s;
	return out;
}

map<string, char*> fm = {
	{ "\xf1%s's %s revealed \x99 %c\xf0", u8"\xf1发现了 %s 的 %s \x99 %c\xf0" },
	{ "%c + %d, \xf1""total %s\xf0 %d \x99 %d\n", u8"%c + %d, \xf1总数额： %s\xf0 %d \x99 %d\n" },
	{ "LOOT in the car: %s\n\n", u8"车里物品: %s\n\n" }
};

int formatStr(char *buf, char *fmt, va_list argv)
{
	if (fm.count(fmt))
		fmt = fm[fmt];
	return vsprintf(buf, fmt, argv);
}

int formatStr(char* buffer, char* fmt, ...)
{
	va_list v1;
	va_start(v1, fmt);
	int r = formatStr(buffer, fmt, v1);
	va_end(v1);
	return r;
}

int formatStr(char *buf, int length, char *fmt, va_list argv)
{
	return formatStr(buf, fmt, argv);
}

char* lootNames[] = 
{
	u8"无",
	u8"食物",
	u8"汽油",
	u8"药物",
	u8"手枪弹药",
	u8"步枪弹药",
	u8"散弹弹药",
	u8"垃圾",
};

char* getLootName(int id)
{
	if (id < 0)
		id = 0;
	else if (id > 7)
		id = 7;
	return lootNames[id];
}

char* statNames[] =
{
	u8"精神健康",
	u8"个人风格",
	u8"冷静程度",
	u8"魅力",
	u8"嘴炮",
	u8"忠诚",
	u8"医学",
	u8"汽修",
	u8"射击",
	u8"力量",
	u8"敏捷",
	u8"健康",
	u8"生命值",
	"invalid"
};

char* getStatName(unsigned int id)
{
	if (id > 13)
		id = 13;
	return statNames[id];
}

void GLAPIENTRY Render(GLenum mode, GLsizei count, GLenum type, const void *indices)
{
	if(renderIndex)
		glDrawElements(mode, renderIndex * 6, type, indices);

	//glDrawElements(mode, count, type, indices);
	
	font.Draw();
	font.Clear();

	glBindTexture(GL_TEXTURE_2D, renderer->texid);

	glVertexPointer(3, 0x1406u, 0x20, (char*)(renderer->pvertexbuffer));
	glTexCoordPointer(2, 0x1402u, 0x20, (char*)(renderer->pvertexbuffer) + 0x0C);
	glColorPointer(4, 0x1401u, 0x20, (char*)(renderer->pvertexbuffer) + 0x10);
	glDrawElements(mode, count - renderIndex * 6, type, (short*)indices + renderIndex * 6);

	font.FGDraw();
	font.FGClear();

	if (bg.getID())
	{
		if (SDL_GetTicks() > 3000)
			bgColor.a *= 0.99;

		bg.AddQuad(0, 0, nullptr, &bgColor);
		bg.Draw();
		bg.Clear();
	}
	renderIndex = 0;

	glBindTexture(GL_TEXTURE_2D, renderer->texid);
}

//void GLAPIENTRY LoadTex(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
//{
//	GLubyte* buf = new GLubyte[width * height * 16];
//	memset(buf, 0, sizeof(buf));
//	for (int y = 0; y < height; y++)
//		for (int x = 0; x < width; x++)
//		{
//			GLushort p = ((GLushort*)pixels)[y * width + x];
//			//buf[(y * width * 2 + x) * 4] = p >> 11 << 3;
//			//buf[(y * width * 2 + x) * 4 + 1] = p << 5 >> 11 << 3;
//			//buf[(y * width * 2 + x) * 4 + 2] = p << 10 >> 11 << 3;
//			//buf[(y * width * 2 + x) * 4 + 3] = (p & 1) ? 0xff : 0;
//
//			buf[((y + height) * width * 2 + x) * 4] = p >> 11 << 3;
//			buf[((y + height) * width * 2 + x) * 4 + 1] = p << 5 >> 11 << 3;
//			buf[((y + height) * width * 2 + x) * 4 + 2] = p << 10 >> 11 << 3;
//			buf[((y + height) * width * 2 + x) * 4 + 3] = (p & 1) ? 0xff : 0;
//		}
//
//	*tw = width * 2;
//	*th = height * 2;
//
//	glTexImage2D(target, level, internalformat, width * 2, height * 2, border, format, GL_UNSIGNED_BYTE, buf);
//	delete buf;
//}