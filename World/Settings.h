#pragma once
#include <string>
#include "Render/Camera.h"

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

	Camera * GetCamera();
	Camera * GetSun();
	void LookAt(const glm::vec3 & pos);

private:

	std::shared_ptr<Camera> mCamera;
	std::shared_ptr<Camera> mSun;
};

