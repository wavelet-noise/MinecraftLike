#include "Settings.h"



Settings::Settings()
{
	mCamera = std::make_shared<Camera>();
	mSun = std::make_shared<Camera>();

	LookAt({ 0,0,0 });
}


Settings::~Settings()
{
}

void Settings::LookAt(const glm::vec3 & pos)
{
	mCamera->SetPos(pos + glm::vec3(-20, -20, 10));
	mCamera->LookAt(pos );
}

Camera* Settings::GetCamera()
{
	return mCamera.get();
}

Camera* Settings::GetSun()
{
	return mSun.get();
}
