#include <stdafx.h>
#include <log_manager.h>

// The icky template-specialization / static-initializer for Application's msSingleton attribute
template<> ssuge::LogManager* ssuge::Singleton<ssuge::LogManager>::msSingleton = NULL;


ssuge::LogManager::LogManager(std::string fname)
{
	mFile.open(fname, std::ios::out);
}

ssuge::LogManager::~LogManager()
{
	mFile.close();
}

void ssuge::LogManager::log(std::string msg)
{
	mFile << msg << std::endl;
}
