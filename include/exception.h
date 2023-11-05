#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

namespace ssuge
{
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// @@ A simple exception class for ssuge-related error-reporting.
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class Exception
	{
	// ***** ATTRIBUTES *****
	protected:
		/// The description of the error
		std::string mDescription;

		/// The file the error happened in
		std::string mFilename;

		/// The line on which the error happened
		unsigned int mLine;

	// ***** CONSTRUCTOR ******
	public:
		Exception(std::string desc, std::string file, unsigned int line) :
			mDescription(desc), mFilename(file), mLine(line)
		{

		}
	};

	/// A more convenient way to trigger the exception
#define EXCEPTION(d) throw ssuge::Exception(d, __FILE__, __LINE__)
}

#endif
