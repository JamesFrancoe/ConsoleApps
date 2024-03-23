#include <string.h>
#include <ctype.h>
#include "Include/ConsoleLib.hpp"

char* TranslatePath(_Inout_ char* Path, bool Dir = false)
{
	int i = 0;
	int p = 0;
	for (; Path[i]; i++)
	{
		Path[i] = toupper(Path[i]);
		if (Path[i] == '\\')
			Path[i] = '/';

		if (Path[i] == '.')
		{
			p = i;
		}
	}
	if (Dir)
	{
		while (i --> 0)
		{
			if (Path[i] == '/')
			{
				Path[i] = '\0';
				break;
			}
		}
	}
	return Path + p;
}

char* ConsoleWrapper::GetArgAsPath(int index)
{
	if (index < Argc)
	{
		char* arg = Argv[index];
		char* extension = nullptr;
		
		extension = TranslatePath(arg);

		if (extension)
			return arg;

		return nullptr;
	}
	return nullptr;
}

ConsoleWrapper::ConsoleWrapper(const char* Appname)
{
	this->Appname = Appname;
}

bool ConsoleWrapper::Init(int argc, char** argv)
{
	SetSubtitle();
	this->Argc = argc;
	this->Argv = argv;

	(void)TranslatePath(argv[0], true);
	ExePath = argv[0];

	return true;
}