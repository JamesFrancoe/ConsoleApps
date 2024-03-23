#pragma once
constexpr inline char toUpper(char cur)
{
	if (cur >= 'a' && cur <= 'z')
	{
		cur -= 32;
	}
	return cur;
}
typedef unsigned int FNVHash;

constexpr inline FNVHash HashFNV(const char* word)
{
	if (!word)
		return 0;
	FNVHash ret = 0x811c9dc5; // ofset basis
	for (int i = 0; word[i]; i++)
	{
		ret ^= word[i];
		ret *= 0x01000193ULL;
	}
	return ret;
}
constexpr inline FNVHash HashFNVUPPER(const char* word)
{
	if (!word)
		return 0;
	FNVHash ret = 0x811c9dc5; // ofset basis
	for (int i = 0; word[i]; i++)
	{
		ret ^= toUpper(word[i]);
		ret *= 0x01000193ULL;
	}
	return ret;
}