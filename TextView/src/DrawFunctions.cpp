#include"TextView.hpp"
static inline void WriteToken(const char* token, COLOR c = COLOR::WHITE)
{
	Console::WriteText(token, c);
	putchar(' ');
}
static inline void print(const char* str)
{
	fputs(str, stdout); // faster than printf hopefully
}


void TextView::DrawHorizontalLine()
{
	int newsize = csize.x + 1;
	int rest = newsize % 16;
	newsize /= 16;
	Console::SetTextColor(UIColor);
	for (int i = 0; i < newsize; i++)
	{
		print("----------------");
	}
	for (int i = 0; i < rest; i++)
	{
		print("-");
	}
	Console::SetTextColor(COLOR(Foreground));
}

void TextView::ClearLine()
{
	int newsize = csize.x + 1;
	int rest = newsize % 16;
	newsize /= 16;
	Console::SetTextColor(UIColor);
	for (int i = 0; i < newsize; i++)
	{
		print("                ");
	}
	for (int i = 0; i < rest; i++)
	{
		print(" ");
	}
	Console::SetTextColor(COLOR::WHITE);
}

void TextView::DrawFile()
{
	Console::SetCursorPos(0, TopBarSize);
	int i = 0;
	int LCBackup = LineCount;
	int Space = csize.y - TopBarSize - functionBarSize;
	for (; i < Space; i++)
	{
		int offset = Line + i;
		if ((unsigned)offset >= TotalLines)
		{
			LineCount = LCBackup;
			break;
		}
		Console::WriteFormatted(UIColor, "%0*u | ", MaxLineCLen, offset + 1);

		const Line_s &cur = LineVector[offset];
		if (*cur.linebuffer == '\n')
		{
			putchar('\n');
			continue;
		}

		bool comment = false;

		for (int i = 0; i < cur.Tokens.size(); i++)
		{
			char* token = cur.Tokens[i];
			if (token[0] == '\0')
				break;
			if (token[0] == ' ')
			{
				for (int i = 0; token[i]; i++)
				{
					if (token[i] != ' ')
					{
						token += i;
						break;
					}
					putchar(' ');
				}
			}
			// if one letter
			if (token[1] == '\0') // special case for comment
			{
				if (SyntaxProcess)
				{
					if (!(curFile->syntaxConfig.CheckNumbers && isdigit(token[0]))
						&& !(curFile->syntaxConfig.CheckComments && token[0] == ';'))
					{
						WriteToken(token);
						continue;
					}
				}
			}
			bool printed = false;
			
			if (SyntaxProcess)
			{
				SyntaxConfig &curConfig = curFile->syntaxConfig;
				if (!comment && curConfig.CheckComments)
				{
					for (int i = 0; i < curConfig.CommentPrefixesCount; i++)
					{
						if (strspn(curConfig.CommentPrefixes[i], token))
						{
							comment = true;
							break;
						}
					}
				}

				if (comment)
				{
					WriteToken(token, COLOR(Comment));
					continue;
				}

				if (curConfig.CheckStrings && token[0] == '\"')
				{
					WriteToken(token, COLOR(String));
					printed = true;
					continue;
				}

				if (curConfig.CheckNumbers)
				{
					if (isdigit(token[0]))
					{
						WriteToken(token, COLOR(Number));
						printed = true;
 					}
				}

				if (!printed && SyntaxProcess)
				{
					FNVHash hash = HashFNV(token);
					for (const auto& c : curFile->syntaxList)
					{
						if (hash == c.hash)
						{
							Console::WriteText(token, (COLOR)c.color);
							printed = true;

							if (c.processNext)
							{
								i++;
								if (i < cur.Tokens.size())
								{
									putchar(' ');
									Console::WriteText(cur.Tokens[i], (COLOR)c.next);
								}
							}
							break;
						}
					}
				}

				if (!printed)
				{
					if (curFile->syntaxConfig.TreatGetSetAsFunc)
					{
						char cur = tolower(token[0]);
						if (cur == 's' || cur == 'g')
						{
							cur = tolower(token[1]);
							if (cur == 'e')
							{
								cur = tolower(token[2]);
								if (cur == 't')
								{
									WriteToken(token, COLOR(Function));
									continue;
								}
							}
						}
					}
					COLOR print = COLOR(Foreground);
					if (token[2] != '\0' 
						&& (size_t)i + 1 >= cur.Tokens.size())
					{
						print = COLOR(Variable);
					}
					else
					{
						if (token[0] != '=')
							print = COLOR(Function);
					}
					Console::WriteText(token, print);
				}
				putchar(' ');
				continue;
			}
			WriteToken(token, COLOR(Foreground));
		}
		putchar('\n');
	}
	LineCount = i;
}

void TextView::DrawTopBar()
{
	Console::SetCursorPos(0, 1);
	Console::SetTextColor(UIColor);

	printf("Lines: %u ", TotalLines);
	printf("Words: %u ", TotalWords);

	Console::SetCursorPos(csize.x / 2, 1);
	print(filename);

	Console::SetCursorPos(0, 2);
	DrawHorizontalLine();

	Console::SetTextColor(COLOR(Foreground));
	updateTopBar = false;
}

void TextView::DrawFunctionbar()
{

	// draw divider
	Console::SetCursorPos(0, csize.y - functionBarSize);
	
	DrawHorizontalLine();

	// draw messages
	{
		int row = csize.y - 2;
		for (size_t i = Messages.size(); i --> 0;)
		{
			Console::SetCursorPos(0, row);

			if (Messages[i])
			{
				print(Messages[i]);
				row--;
			}
		}
	}
}