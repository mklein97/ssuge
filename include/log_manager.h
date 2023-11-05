#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_
#include <stdafx.h>
#include <singleton.h>

// A "convenience macro" to make accessing the LogManager singleton a bit easier.
#define LOG_MANAGER ssuge::LogManager::getSingletonPtr()

namespace ssuge
{
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// The LogManager class is responsible for maintaining
	/// a log of all ssuge-related log messages in a single file which
	/// stays open for the life of the application.
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class LogManager : public Singleton<LogManager>
	{
	// ***** ATTRIBUTES *****
	protected:
		/// The file object, which is opened for writing upon creating our
		/// log manager and is closed when we shut down
		std::fstream mFile;

	// ***** CONSTRUCTOR / DESTRUCTOR *****
	public:
		/// fname is the file name we want to open
		LogManager(std::string fname);

		/// Closes the log file.
		~LogManager();

	// ***** METHODS *****
	public:
		/// Writes a line of text to the log file.
		void log(std::string msg);
	};
}

#endif
