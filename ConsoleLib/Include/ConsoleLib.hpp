#pragma once

struct Coordinate
{
	short x = 0;
	short y = 0;
};
enum class COLOR : unsigned char
{
	BLACK,
	BLUE,
	GREEN,
	AQUA,
	RED,
	PURPLE,
	GOLD,
	GREY = 7,
	DEFAULT = 7,
	DARKGREY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTAQUA,
	LIGHTRED,
	LIGHTPURPLE,
	LIGHTYELLOW,
	WHITE
};

struct RGB
{
	unsigned char Red = 0;
	unsigned char Green = 0;
	unsigned char Blue = 0;
};

class ConsoleWrapper
{
public:
	ConsoleWrapper(const char* Appname);
	bool Init(int argc, char** argv);
	char* GetArgAsPath(int index);

	// Output
	static Coordinate GetConsoleSize();
	static void ClearScreen(short offsetY = 0, short length = 0); 
	static void SetCursorPos(short x, short y);
	bool SetSubtitle(const char* subtitle = nullptr);
	static void SetPalletteEntry(COLOR slot, RGB color);
	static void SetTextColor(COLOR color);
	static void Refresh();
	static void WriteText(const char* text, COLOR color = COLOR::WHITE);
	static void WriteFormatted(COLOR color, const char* format, ...);
	
	// Input
	static char* GetString(char* buffer, size_t bufsize,bool print = true);
	
	static char GetChar(bool print = false);

private:
	const char* Appname = nullptr;
	const char* ExePath = nullptr;

	int Argc = 0;
	char** Argv = nullptr;
};

typedef ConsoleWrapper Console;