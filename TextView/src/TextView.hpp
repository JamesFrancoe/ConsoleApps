#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <vector>
#include "Fnvhash.hpp"
#include "ConsoleLib.hpp"

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

class TextView
{
public:
	TextView(Console *instance)
	{
		InitVSColorScheme();
		this->instance = instance;
	}
	void Start(char* filename);

	COLOR UIColor = COLOR::LIGHTYELLOW;
	SyntaxConfig syntaxConfig;
	template<size_t size>
	inline void PushSyntaxList(const Syntax(&list)[size])
	{
		for (size_t i = 0; i < size; i++)
		{
			syntaxList.push_back(list[i]);
		}
	}
	template<size_t size>
	inline void PushCommentPrefixList(const char*(&list)[size])
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
		keywordList.push_back(keyword);
	}
private:
	std::vector<Syntax> syntaxList;
	std::vector<SyntaxKeyword> keywordList;
	bool SyntaxProcess = false;

	char* filename = nullptr;
	static const int MaxMessages = 3;
	std::vector<char*> Messages;
	std::vector<Line_s> LineVector;
	Console *instance;
	void PushMessage(const char* message, ...);
	void ProgramLoop();
	void ReadLines();
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