#pragma once
#include <boost/locale.hpp>
#include "tools/Log.h"

using namespace boost::locale;

class Localize
{
public:
	static Localize &instance()
	{
		static Localize loc;

		return loc;
	}

	generator gen;

	void Init()
	{
		gen.add_messages_path(".");
		gen.add_messages_domain("hello");

		// Generate locales and imbue them to iostream
		std::locale::global(gen(""));
		//LOG(info) << "Current localization" << std::locale();
		LOG(info) << translate("localization example");
	}
};
