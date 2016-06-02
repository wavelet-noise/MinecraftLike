#ifndef AutoVersion_h__ 
#define AutoVersion_h__ 
#include <string>
#define Ver "v0.1-1-g6789d08";
#define Time "1464895487";
class AutoVersion { 
public: 
	static std::string GetTitle();
private:
	static std::string title_;
static bool ready_;
};
#endif // AutoVersion_h__  
