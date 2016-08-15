#pragma once
#ifndef StringIntern_h__
#define StringIntern_h__

#include <string>
#include <boost/flyweight.hpp>

//using StringIntern = std::string;
using StringIntern = boost::flyweight<std::string>;

StringIntern operator+(const StringIntern &a, const StringIntern &b);

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void serialize(Archive & ar, StringIntern& si, const unsigned int)
		{
			ar & si.get();
		}
	}
}

#endif // StringIntern_h__
