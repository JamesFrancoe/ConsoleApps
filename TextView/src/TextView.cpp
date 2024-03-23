#include "TextView.hpp"

void TextView::ReadLines()
{
	if (!gFile)
		return;

	int Line = 0;
	while (true)
	{
		Line_s current;
		char* buffer = current.linebuffer;
		std::vector<char*>* Tokens = &current.Tokens;
		char* token = nullptr;
		current.fileposition = ftell(gFile);
		if (fgets(current.linebuffer, current.maxBuffer, gFile))
		{
			current.linenumber = ++Line;
			bool qoute = false;
			token = (char*)calloc(current.maxToken, 1);
			if (NULL != token || buffer != NULL)
			{
				int j = 0;
				for (int i = 0; buffer[i]; i++)
				{
					if (j == current.maxToken)
					{
						unsigned int newsize = j + 32U;
						token = (char*)realloc(token, newsize);
						for (int ii = j; ii < newsize ; ii++)
						{
							token[ii] = '\0';
						}
					}
					if (buffer[i] == '\n')
					{
						if (token)
						{
							if (token[0] == '\0')
								free(token);
							else
							{
								Tokens->push_back(token);
								TotalWords++;
							}
						}
						break;
					}

					if (buffer[i] == '\t' 
						|| buffer[i] == '{' || buffer[i] == '}')
					{
						char* cToken = (char*)malloc(2);
						if (cToken)
						{
							cToken[0] = buffer[i];
							cToken[1] = '\0';
							if (token && token[0])
							{
								Tokens->push_back(token);
								token = (char*)calloc(current.maxToken, 1);
								j = 0;
							}
							Tokens->push_back(cToken);
							continue;
						}
					}
					if (buffer[i] == '\"')
						qoute = !qoute;

					if (qoute || buffer[i] != ' ')
					{
						token[j] = buffer[i];
						if (buffer[i + 1] != ' ')
						{
							j++;
							continue;
						}
					}

					if (!*token)
						continue;

					Tokens->push_back(token);
					TotalWords++;
					token = (char*)calloc(current.maxToken, 1);
					if (!token)
						throw "err";
					j = 0;
				}
			}
		}
		else
		{
			break;
		}
		
		if (Tokens->size() == 0 
				&& *current.linebuffer != '\n' 
				&& token && token[0] != '\0')
			Tokens->push_back(token);

		LineVector.push_back(current);
	}
	TotalLines = Line;
	fclose(gFile);
}

void TextView::ProgramLoop()
{
	bool updateFunctionOnly = false;
	while (true)
	{
		csize = Console::GetConsoleSize();
		// updateALL
		Console::ClearScreen();
		DrawTopBar();
		DrawFunctionbar();
		DrawFile();

		Console::SetCursorPos(0, csize.y - 1);
		Console::WriteText("> ");
		char buffer[200]{}; 
		Console::GetString(buffer, 200);
		HandleCommand(buffer);
	}
}
static bool fExists(char* filename)
{
	FILE* temp = 0; 
	fopen_s(&temp, filename, "r");
	if (temp)
	{
		fclose(temp);
		return true;
	}
	return false;
}

constexpr FNVHash echo = HashFNVUPPER("echo");
constexpr FNVHash Theme = HashFNVUPPER("Theme");
constexpr FNVHash cLineWidth = HashFNVUPPER("LineWidth");
constexpr FNVHash Open = HashFNVUPPER("open");
void TextView::HandleCommand(char* command)
{
	
	if (command[0] == '\0')
	{
		updateTopBar = true;
		return;
	}
	if (command[0] == 'c' || command[0] == 'q')
	{
		if (command[1])
		{
			if (tolower(command[1] == 'o'))
				goto PARAMARGS;

			if (tolower(command[1]) == 'l' && tolower(command[2]) == 's')
			{
				for (char* msg : Messages)
				{
					free(msg);
				}
				Messages.clear();
				return;
			}
		}
		else
		{
			exit(0);
		}
	}
	if (command[0] == 't')
	{
		if (command[1] && command[1] != 'o')
			goto PARAMARGS;

		if (Line)
		{
			OutOfBounds = false;
			Line = 0;
			return;
		}
	}
	if (command[0] == 'b')
	{
		Line = TotalLines - LineCount;
		return;
	}
	if (command[0] == 'd')
	{
		unsigned int i = 1;
	repeat:
		Line += LineCount;
		if (Line >= TotalLines - LineCount)
		{
			Line = TotalLines - LineCount;
			return;
		}
		if (tolower(command[i++]) == 'd')
			goto repeat;

		return;
	}
	if (command[0] == 'u')
	{
		if (Line > 0)
		{
			unsigned int i = 1;
		upRepeat:
			Line -= LineCount;
			if ((signed)Line <= 0)
			{
				Line = 0;
				return;
			}

			if (tolower(command[i++]) == 'u')
				goto upRepeat;
			
			return;
		}
	}

PARAMARGS:
	#define return() for (char* current : args) free(current); return;
	char* cur = (char*)calloc(86, 1);
	auto args = [&]()
	{
		std::vector<char*> args;
		int j = 0;
		for (int i = 0; command[i]; i++)
		{
			if (command[i] != ' ')
			{
				cur[j] = command[i];
				j++;
				continue;
			}
			if (!cur[0])
				continue;

			args.push_back(cur);
			cur = (char*)calloc(86, 1);
			j = 0;
		}
		if (cur && cur[0])
			args.push_back(cur);
		return args;
	}();
	if (args.size() > 1)
	{
		FNVHash hCommand = HashFNVUPPER(args[0]);

		if (command[0] == 'l')
		{
			char* arg = args[1];
			size_t number = atoll(arg);
			if (number < TotalLines && number)
			{
				Line = number - 1;
			}
			return();
		}
		
		if (  command[0] == 'c'
		   && command[1] == 'o'
		   && command[2] == 'n')
		{
			if (HashFNVUPPER(args[1]) == cLineWidth) // LineWidth
			{
				if (args.size() < 2)
				{
					return();
				}

				int width = atoi(args[2]);
				if (width)
				{
					MaxLineCLen = width;
					return();
				}
			}
		}

		if (hCommand == echo)
		{
			char msgbuffer[300]{};
			char word[20]{};
			for (int i = 1; i < args.size(); i++)
			{
				sprintf_s(word, "%s ", args[i]);
				strcat_s(msgbuffer, word);
			}
			if (msgbuffer)
				PushMessage(msgbuffer);

			return();
		}
		if (hCommand == Theme)
		{
			if (args.size() != 1)
			{
				if (toupper(args[1][0]) == 'V')
				{
					if (args[1][1] && toupper(args[1][1]) != 'S')
					{
						return();
					}

					InitVSColorScheme();
					updateTopBar = true;
					return();
				}
				if (toupper(args[1][0]) == 'D')
				{
					if (args[1][1] && toupper(args[1][1]) != 'R')
					{
						return();
					}

					InitDraculaColorScheme();
					updateTopBar = true;
					return();
				}
			}

		}
		if (hCommand == Open)
		{
			if (args[1])
			{
				if (fExists(args[1]))
				{
					for (auto &cur : LineVector)
					{
						for (char* str : cur.Tokens)
							free(str);
					}
					LineVector.clear();

					free(args[0]);

					Start(args[1]);
				}
				else
				{
					PushMessage("Could not open file \"%s\"", args[1]);
					return();
				}
			}
		}
	}
	else
	{
		free(args[0]);
	}
	Console::SetCursorPos(0, csize.y - 1);
	ClearLine();
	Console::SetCursorPos(0, csize.y - 1);

	Console::WriteText("> ");
	Console::GetString(command, 200);
	HandleCommand(command);
}

static char* getFileNameOnly(char* filename)
{
	size_t index = strlen(filename);
	while (index --> 0)
	{
		if (filename[index] == '/' || filename[index] == '\\')
		{
			return filename + (index + 1);
		}
	}
	return nullptr;
}
static inline char* strlow(char* in)
{
	for (int i = 0; in[i]; i++)
	{
		in[i] = tolower(in[i]);
	}
	return in;
}
static inline char* strupp(char* in)
{
	for (int i = 0; in[i]; i++)
	{
		in[i] = toupper(in[i]);
	}
	return in;
}
void TextView::Start(char* filename)
{
	if (filename)
	{
		fopen_s(&gFile, filename, "r");
		if (!gFile)
		{
			PushMessage("Could not open file %s", filename);
		}
		else PushMessage("Opened file %s", filename);
		
		this->filename = getFileNameOnly(filename);
		ReadLines();
	}
	else PushMessage("Open a file with the command Open {file}");

	PushMessage("Type [H]elp for list of commands. Press enter to rescale UI");
	if (this->filename)
	{
		memcpy_s(lwFilename, 20, this->filename, strnlen(this->filename, 20));
		strlow(lwFilename);

		for (auto& cur : FileSyntaxList)
		{
			if (strstr(lwFilename, cur.extension))
			{
				curFile = &cur;
				SyntaxProcess = true;
				break;
			}
		}
	}
	ProgramLoop();
}

void TextView::PushMessage(const char* format, ...)
{
	va_list args;
	int len;
	char* buffer;

	if (Messages.size() == MaxMessages)
	{
		free(Messages[0]);
		Messages.erase(Messages.begin());
	}

	va_start(args, format);
	len = _vscprintf(format, args) // _vscprintf doesn't count
		+ 1; // terminating '\0'
	buffer = (char*)malloc(len * sizeof(char));
	if (NULL != buffer)
	{
		vsprintf_s(buffer, len, format, args);
		Messages.push_back(buffer);
	}
	va_end(args);
}
