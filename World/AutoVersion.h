#ifndef AutoVersion_h__ 
#define AutoVersion_h__ 
#include <string>
#define Ver "v0.1-4-g15a9aa1";
#define Time "1464991327";
class AutoVersion { 
public: 
	static std::string GetTitle();
private:
	static std::string title_;
static bool ready_;
};
#endif // AutoVersion_h__  
