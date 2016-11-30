#pragma once
#include <gui\WindowBase.h>
#include <tools\StringIntern.h>

struct Room;
class World;

class Currency
{
public:
	void AddCurrency(int count);
	int GetCurrency() const;
	static int GetSaldo();
	void Update(float dt);

	static Currency &Get()
	{
		static Currency w;
		return w;
	}

private:
	int currency = 0;
};

class WindowOverlay : public WindowBase
{
public:

	WindowOverlay();

	// Унаследовано через WindowBase
	void Draw(glm::vec2 mainwin_size, float gt) override;

	static WindowOverlay &Get()
	{
		static WindowOverlay wr;
		return wr;
	}
};