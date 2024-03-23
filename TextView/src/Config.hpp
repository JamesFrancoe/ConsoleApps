#pragma once

enum ConfigMode
{
	Open,
	Parse,
	Overwrite
};
struct ConfigSaver
{
	struct confObj
	{
		const char* name;
		const char* data;
	};
	const char* filename;

	ConfigSaver(const char* FileName, ConfigMode mode)
	{
		this->filename = FileName;
	}

	/*
		ConfigSaver STX("TextView.config", Overwrite);
		Settings list[] =
		{
			"General Settings",
			{
				NAME("UI"),
				SETTING("Theme", currentTheme),
				SETTING("LineWidth", CLineWidth),
			},
			
			"Syntax",
			{
				NAME("SCP"),
				SETTING("Conditions", Function),
				SETTING("Actions",    Function),
				SETTING("state",      Keyword, Class),
				SETTING("if",         Control, Variable)
			}
		}
		STX.push(list);
		STX.close();
********************************************************************************
									TextView.config
		Labels
		{
			Function 14
			Keyword 9
			Class 3
			Control 13
			Variable 11
		}

		[General Settings]
		
		UI
		{
			Theme     VS
			LineWidth 8
		}

		[Syntax]

		SCP
		{
			Conditions $Function
			Actions    $Function
			state      $Keyword $Class
			if         $Control $Variable
		}

*******************************************************************************
	*/
};