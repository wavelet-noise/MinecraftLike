#pragma once
class Settings
{
public:
	Settings();
	~Settings();

	static Settings Get()
	{
		static Settings s;
		return s;
	}

	inline bool IsDebug() const
	{
		return debug;
	}

	bool debug = true;
};

