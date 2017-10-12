// FILE: io.h
#ifndef _io_h
#define _io_h

#include "process.h"
#include <list>
#include <string>


// It is assumed that no content switching time required for writing into IO or ready queue. 
class IO {

	public:

		// CONSTRUCTOR
		IO();

		// ACCESSOR
		std::string PIDs();

		// MODIFIER
		void push(Process *p);
		void pop();
		void tick();


	private:

		// REPRESENTATION
		std::list<Process *> processes_;


};

#endif
