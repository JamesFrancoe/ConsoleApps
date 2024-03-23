#include "WIN32.hpp"

char ConsoleWrapper::GetChar(bool print)
{
	INPUT_RECORD inputbuffer{};
	DWORD ReadCounter = 0;

	while (true)
	{
		ReadConsoleInputA(hInput, &inputbuffer, 1, &ReadCounter);

		auto evt = inputbuffer.Event.KeyEvent;

		if (evt.bKeyDown)
			continue;

		char Key = tolower(evt.uChar.AsciiChar);

		if (print)
		{
			printf("%c", Key);
		}

		return Key;
	}
}

char* ConsoleWrapper::GetString(char* buffer, size_t size, bool print)
{
	gets_s(buffer, size);

	return buffer;
}