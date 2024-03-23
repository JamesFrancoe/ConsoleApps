#include <stdio.h>
#include <stdlib.h>
#include "ConsoleLib.hpp"

#include "HexView.hpp"

int main(int argc, char** argv)
{
	Console app("HexView");
	if (!app.Init(argc, argv))
	{
		return 1;
	}

	char* file = app.GetArgAsPath(1);
	if (file)
	{
		app.SetSubtitle(file);
	}

	HexView Program(file);


	Console::GetChar();
}