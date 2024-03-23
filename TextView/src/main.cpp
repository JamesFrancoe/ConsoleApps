#include <stdio.h>
#include <stdlib.h>
#include "ConsoleLib.hpp"
#include "TextView.hpp"

int main(int argc, char** argv)
{
	Console app("TextView");
	if (!app.Init(argc, argv))
	{
		return 0;
	}
	char* file = app.GetArgAsPath(1);
	if (file)
	{
		app.SetSubtitle(file);
	}

	TextView Program(&app);
	if (file && strstr(file, ".SCP"))
	{
		constexpr Syntax SyntaxWords[] =
		{
//          STRING              COLOR          NEXT
			{"state",           SC::Keyword,   SC::Class    },
			{"Conditions",      SC::Function                },
			{"ReferenceScript", SC::Function                },
			{"Actions",         SC::Function                },
			{"if",              SC::Control,   SC::Variable },
			{"true",            SC::Keyword                 },
			{"false",           SC::Keyword                 },
			{"and",             SC::Keyword                 },
			{"goto",            SC::Control,   SC::Class    },
			{"Setstate",        SC::Function,  SC::Class    },
			{"Dist",            SC::Keyword                 },
			{"PARAM",           SC::Control                 }
		};
		Program.PushSyntaxList(SyntaxWords);

		const char* Commentprefixes[] = {";", "//", "#"};
		Program.PushCommentPrefixList(Commentprefixes);

		Program.syntaxConfig.TreatGetSetAsFunc = true;
	}
	Program.Start(file);
} 