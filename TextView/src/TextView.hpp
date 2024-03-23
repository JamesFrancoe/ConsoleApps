#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <vector>
#include "Fnvhash.hpp"
#include "ConsoleLib.hpp"

#pragma warning( push )
#pragma warning( disable : 4477)
constexpr RGB HexToRGB(const char* bytes)
{
	RGB ret;
	if (bytes[0] != '#')
	{
		return ret;
	}
	sscanf_s(bytes, "#%02x%02x%02x", &ret.Red,&ret.Green, &ret.Blue);
	return ret;
}
struct Line_s
{
	uint32_t linenumber = 0;
	uint32_t fileposition = 0;

	static const size_t maxToken = 32;
	std::vector<char*> Tokens;

	static const size_t maxBuffer = 150;
	char linebuffer[maxBuffer]{};
};

enum SC : unsigned char
{
	Background = COLOR::BLACK,
	Foreground = COLOR::DEFAULT,
	Function = COLOR::LIGHTYELLOW,
	Comment = COLOR::GREEN,
	Keyword = COLOR::LIGHTBLUE,
	String = COLOR::GOLD,
	Class = COLOR::AQUA,
	Variable = COLOR::LIGHTAQUA,
	Number = COLOR::LIGHTGREEN,
	Control = COLOR::LIGHTPURPLE,
};
#define COLOR(SCS) (COLOR)SC::SCS

struct Syntax
{
	FNVHash hash = 0;
	SC color  = Foreground;

	bool processNext = false;
	SC next = Background;
	constexpr Syntax(const char* word, SC color)
	{
		this->hash = HashFNV(word);
		this->color = color;
	}
	constexpr Syntax(const char* word, SC color, SC next)
	{
		this->hash = HashFNV(word);
		this->color = color;
		this->processNext = true;
		this->next = next;
	}
};

struct SyntaxConfig
{
	char *CommentPrefixes[5]{0,0,0,0,0};
	int CommentPrefixesCount = 0;
	bool CheckComments = true;
	bool CheckNumbers = true;
	bool CheckStrings = true;
	bool TreatGetSetAsFunc = false;
	SC Function  = SC::Function;
	SC Comment   = SC::Comment;
	SC Keyword   = SC::Keyword;
	SC String    = SC::String;
	SC Class     = SC::Class;
	SC Variable  = SC::Variable;
	SC Number    = SC::Number;
	SC Control   = SC::Control;
	SC Foreground = SC::Foreground;
};


struct SyntaxKeyword
{
	FNVHash keyword;
	COLOR color1;
	COLOR color2;
	SyntaxKeyword(const char* Keyword, COLOR color, COLOR next = COLOR::BLACK)
	{
		this->keyword = HashFNV(Keyword);
		this->color1 = color;
		this->color2 = next;
	}
};

struct FileSyntax
{
	char extension[20]{};
	std::vector<SyntaxKeyword> Keywords;
	std::vector<Syntax> syntaxList;
	SyntaxConfig syntaxConfig;


	inline FileSyntax(const char* extension)
	{
		strlow(extension, this->extension);
	}
	template<size_t size>
	inline void PushSyntaxList(const Syntax(&list)[size])
	{
		for (size_t i = 0; i < size; i++)
		{
			syntaxList.push_back(list[i]);
		}
	}
	template<size_t size>
	inline void PushCommentPrefixList(const char* (&list)[size])
	{
		syntaxConfig.CheckComments = true;
		syntaxConfig.CommentPrefixesCount = size;
		if (size < 5)
		{
			memcpy_s(syntaxConfig.CommentPrefixes, sizeof(char*) * 5,
				list, sizeof(char*) * size);
		}

	}
	inline void PushKeyword(SyntaxKeyword keyword)
	{
		Keywords.push_back(keyword);
	}
	inline char* strlow(const char* in, char* out)
	{
		for (int i = 0; in[i] && i < 20; i++)
		{
			out[i] = tolower(in[i]);
		}
		return out;
	}
};

class TextView
{
public:
	TextView(Console *instance)
	{
		InitVSColorScheme();
		this->instance = instance;
	}
	void Start(char* filename);

	inline void Register(FileSyntax& fs)
	{
		if (fs.extension != nullptr)
		{
			FileSyntaxList.push_back(fs);
			return;
		}
		fprintf(stderr, "Tried to register FileSyntax but it had no extension.");
	}
	COLOR UIColor = COLOR::LIGHTYELLOW;

	
private:

	bool SyntaxProcess = false;
	std::vector<FileSyntax> FileSyntaxList;
	FileSyntax* curFile = nullptr;
	

	char* filename = nullptr;
	char  lwFilename[20]{};
	static const int MaxMessages = 3;
	std::vector<char*> Messages;
	std::vector<Line_s> LineVector;
	Console *instance;
	void PushMessage(const char* message, ...);
	void ProgramLoop();
	void ReadLines();
#define SETPALLETE(SC, RGB) Console::SetPalletteEntry(COLOR(SC), HexToRGB(RGB))

	inline void InitVSColorScheme()
	{
		UIColor = COLOR::LIGHTYELLOW;
		Console::SetPalletteEntry(COLOR(Background), { 12, 12, 12 });
		Console::SetPalletteEntry(COLOR(Foreground), {204, 204, 204});
		Console::SetPalletteEntry(COLOR(Function), { 220, 220, 171 });
		Console::SetPalletteEntry(COLOR(String), { 214, 157, 133 });
		Console::SetPalletteEntry(COLOR(Keyword), { 86, 156, 214 });
		Console::SetPalletteEntry(COLOR(Class), { 78, 201, 176 });
		Console::SetPalletteEntry(COLOR(Variable), { 156, 220, 254 });
		Console::SetPalletteEntry(COLOR(Number), { 181, 206, 168 });
		Console::SetPalletteEntry(COLOR(Comment), { 35,252,35 });
		Console::SetPalletteEntry(COLOR(Control), { 216, 160, 223 });
	}

	inline void InitDraculaColorScheme()
	{
		UIColor = COLOR::LIGHTPURPLE;
		Console::SetPalletteEntry(COLOR(Background), { 40, 42, 54 });
		Console::SetPalletteEntry(COLOR(Foreground), { 248, 248, 242 });
		Console::SetPalletteEntry(COLOR(String), { 241, 250, 140 });
		Console::SetPalletteEntry(COLOR(Comment), { 98, 114, 164 });
		Console::SetPalletteEntry(COLOR(Class), { 139, 233, 253 });
		Console::SetPalletteEntry(COLOR(Variable), { 80, 250, 123 });
		Console::SetPalletteEntry(COLOR(Control), { 189, 147, 249 });
		Console::SetPalletteEntry(COLOR(Keyword), { 255, 121, 198 });
		Console::SetPalletteEntry(COLOR(Function), { 255, 121, 198 });
	}
	inline void InitTokyoNightColorScheme()
	{
		UIColor = COLOR::DEFAULT;
		SETPALLETE(Background, "#1a1b26");
		SETPALLETE(Foreground, "#a9b1d6");
		SETPALLETE(String,     "#9ece6a");
		SETPALLETE(Comment,    "#565f89");
		SETPALLETE(Number,     "#ff9e64");
		SETPALLETE(Control,    "#bb9af7");
		SETPALLETE(Variable,   "#c0caf5");
		SETPALLETE(Function,   "#7aa2f7");
		SETPALLETE(Class,      "#c0caf5");
		SETPALLETE(Keyword,    "#a9b1d6");
	}
	void HandleCommand(char* command);
	void DrawHorizontalLine();
	void ClearLine();
	void DrawFile();
	bool updateTopBar = true;
	void DrawTopBar();
	void DrawFunctionbar();
	Coordinate csize;
	FILE* gFile = 0;
	static const int TopBarSize = 3;
	bool  OutOfBounds = false; 
	uint32_t   Line           = 0;
	uint32_t   TotalLines     = 0;
	uint32_t   TotalWords     = 0;
	uint32_t   LineCount      = 0; 
	uint32_t   MaxLineCLen    = 8;
	static const char functionBarSize = MaxMessages + 2;
	uint32_t   MessageCounter = 0;
};