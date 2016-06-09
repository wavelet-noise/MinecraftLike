#include "AutoVersion.h"
#include <stdio.h>
#include <time.h>
#include <fstream>

std::string AutoVersion::GetTitle()
{
    if(ready_){
        return title_;
    }

	std::ifstream ifs("ver", std::ifstream::in);

    char buff[100];
    std::string s1;
	ifs >> s1;
    time_t t = atol(s1.c_str());

	char c;
	ifs >> c;

    std::string s2;
	ifs >> s2;

    struct tm ts;
    localtime_s(&ts, &t);
    char buf[80];
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S", &ts);
    sprintf_s(buff, "%s %s %s %s\0", "Alpha Mensae", s2.c_str(), "from", buf);
    title_ = buff;
    ready_ = true;
    return title_;
}

bool AutoVersion::ready_ = false;
std::string AutoVersion::title_;