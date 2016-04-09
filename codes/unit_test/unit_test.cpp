#include <iostream>

#include "gtest/gtest.h"
#include "core_test.h"
#include "engine_test.h"
#include "core/log.h"
#ifdef GFX_TEST

#endif // GFX_TEST

GTEST_API_ int main(int argc, char **argv) {

	engine::InitEngine();
 	tng::Log* log = new tng::ConsoleLog;
 	tng::Log::SetLog(log);
	tng::Log::GetLog()->Printf(tng::Log::SYS_CHN,"starting");
// 
     testing::InitGoogleTest(&argc, argv);
     int ret = RUN_ALL_TESTS();
 	std::cout<<std::string("free memory :")<<tng::OS::GetFreeMemory()<<std::endl;
// 	
 #ifdef _WIN32
 	system("pause");
 #endif
 	delete log;
    return ret;
}
