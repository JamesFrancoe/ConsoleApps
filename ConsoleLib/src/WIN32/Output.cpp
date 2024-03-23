#include "WIN32.hpp"
#include <stdarg.h>
void ConsoleWrapper::SetTextColor(COLOR color)
{
	SetConsoleTextAttribute(hOutput, (BYTE)color);
}
void ConsoleWrapper::ClearScreen(short offsetTop, short offsetBottom)
{
	system("cls");
	return;
	COORD origin = {0, offsetTop};
	Coordinate size = ConsoleWrapper::GetConsoleSize();
	DWORD consize = 0;
	DWORD written = 0;
	if (!offsetBottom)
		consize = size.x * size.y;
	else
		consize = (size.x * size.y) - ((size.x * offsetTop) + (size.x * offsetBottom));

	bool ret = FillConsoleOutputCharacterA(hOutput,' ', consize, origin, &written);
	if (!ret)
	{
		throw "err";
	}
}

void ConsoleWrapper::WriteText(const char* text, COLOR textcolor)
{
	SetTextColor(textcolor);

	fputs(text, stdout);

	SetTextColor(COLOR::DEFAULT);
}

void ConsoleWrapper::WriteFormatted(COLOR textcolor, const char* format, ...)
{
	SetTextColor(textcolor);
	va_list args;
	va_start(args, format);
	vprintf_s(format, args);
	va_end(args);
	SetTextColor(COLOR::DEFAULT);
}

static COLORREF RGBTOCOLORREF(RGB in)
{
	return (((in.Blue << 8) + in.Green) << 8) + in.Red;
}
void ConsoleWrapper::SetPalletteEntry(COLOR slot, RGB color)
{
	CONSOLE_SCREEN_BUFFER_INFOEX output{};
	output.cbSize = sizeof CONSOLE_SCREEN_BUFFER_INFOEX;
	if (!GetConsoleScreenBufferInfoEx(hOutput, &output))
		return;
	// fix because a billion dollar company can't
	output.srWindow.Right += 1;
	output.srWindow.Bottom += 1;

	output.ColorTable[(int)slot] = RGBTOCOLORREF(color);
		
	SetConsoleScreenBufferInfoEx(hOutput, &output);
	return;
}

Coordinate ConsoleWrapper::GetConsoleSize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	GetConsoleScreenBufferInfo(hOutput, &csbi);
	Coordinate ret;
	ret.x = csbi.srWindow.Right;
	ret.y = csbi.srWindow.Bottom - csbi.srWindow.Top;
	return ret;
}

void ConsoleWrapper::Refresh()
{
	SetConsoleActiveScreenBuffer(hOutput);
}
void ConsoleWrapper::SetCursorPos(short x, short y)
{
	SetConsoleCursorPosition(hOutput, { x, y });
}
bool ConsoleWrapper::SetSubtitle(const char* subtitle)
{
	if (!subtitle)
	{
		if (SetConsoleTitleA(Appname))
		{
			return true;
		}
		DWORD Error = GetLastError();
		fprintf(stderr,
			"\nCould not set title. Error: %u (%x)\n"
			"\tAppname was: %s\n", Error, Error, Appname);
		return false;
	}

	char buffer[300]{};
	sprintf_s(buffer, "%s - %s", Appname, subtitle);
	if (SetConsoleTitleA(buffer))
	{
		return true;
	}
	DWORD Error = GetLastError();
	fprintf(stderr,
		"\nCould not set title. Error: %u (%x)\n"
		"\tAppname was %s\n"
		"\tsubtitle was %s\n", Error, Error, Appname, subtitle);
	
	return false;
}