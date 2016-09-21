#pragma once
#include <string>

class Settings
{
public:
	Settings();
	~Settings();

	static Settings &Get()
	{
		static Settings s;
		return s;
	}

	inline bool IsDebug() const
	{
		return debug;
	}

	bool debug = true;
	bool dig_ores = false;
	std::string save_file = "Save\\error.world";
};

