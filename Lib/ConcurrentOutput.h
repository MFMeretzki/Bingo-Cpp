// Definition of the ConcurrentOutput class

#ifndef ConcurrentOutput_class
#define ConcurrentOutput_class

#include <iostream>
#include <string>
#include <mutex>


class ConcurrentOutput
{
	public:
		static void Write(std::string s);
		
		static std::mutex mut;
	
	private:
		ConcurrentOutput() { }
		virtual ~ConcurrentOutput() { }
};


#endif
