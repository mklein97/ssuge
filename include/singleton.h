#ifndef _SINGLETON_H_
#define _SINGLETON_H_
#include <stdafx.h>
#include <exception.h>

namespace ssuge
{
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// A Singleton is a class which only allows one instance
	/// of itself and allows controlled global access.  To make 
	/// an existing class into a singleton, do the following:
	/// 1. Derive your class from Singleton<Foo> (where Foo is your class)
	/// 2. Define the static msSingleton attribute, generally in foo.cpp like this
	///    template<> Foo* Singleton<Foo>::msSingleton = NULL
	/// 3. (optional) Define a convenience macro to simplify the global
	///    access (I usually do this in foo.h)
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	template <class T>
	class Singleton
	{
	// ****** ATTRIBUTES ******
	protected:
		/// The singleton pointer
		static T * msSingleton;

	// ***** CONSTRUCTORS / DESTRUCTORS ******
	public:
		/// The constructor ensures there is only one
		/// instance of this class -- if not a ssuge::Exception is
		/// thrown
		Singleton()
		{
			if (msSingleton != NULL)
				EXCEPTION("You already have a singleton of this type created!");
			msSingleton = (T*)this;
		}

		/// The destructor just resets the msSingleton attribute to
		/// NULL (in case we choose to make another later)
		~Singleton()
		{
			msSingleton = NULL;
		}

	// ***** GETTERS / SETTERS *****
	public:
		/// Gets a pointer to the singleton (or NULL if it hasn't
		///  been created).
		static T * getSingletonPtr()
		{
			return msSingleton;
		}

		/// Gets a reference to the singleton (a little more dangerous
		///  than getSingletonPtr -- if the singleton hasn't been
		///  created, my code raises an exception [to prevent
		///  de-referencing a NULL pointer])
		static T & getSingleton()
		{
			if (msSingleton == NULL)
				EXCEPTION("No singleton to get a reference from");
		}

	};
}

#endif
