// FILE: main.cpp

#include <string>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "clock.h"
#include "process.h"
#include "process_queue.h"
#include "memory.h"
#include "output.h"

int algorithm(const std::string& filename, const std::string& mode);

int main(int argc, char* argv[]) {
	if(argc != 2){
        std::cerr << "USAGE: " << argv[0] << " <input file>" << std::endl;
		return -1;
    }

	//algorithm(argv[1],"next");
	//algorithm(argv[1],"first");
	//algorithm(argv[1],"best");
	algorithm(argv[1],"non");

	return 0;
}


int algorithm(const std::string& filename, const std::string& mode)
{
    ProcessQueue aq; // arrival queue
    Memory mem;
    Clock c;
    Output opt ;

    // load input file
    if (aq.load_file(filename) != 0) {
    	return -1;
    }
    
    opt.PrintStart( mode , c.rtime() ) ;
    //mem.print();
    while (!aq.empty() || !mem.empty()) {
    //while (!aq.empty()) {
    	int arr_time = aq.next_time(); // when next process arrives
    	int fin_time = mem.next_time(); // when next process finishes

    	while (c.time() != arr_time && c.time() != fin_time) {
    		c.tick();
    	}

    	if (c.time() == fin_time) { // remove finished process(es) from memory
			mem.pop( mode , opt, c );
    	}
    	if (c.time() == arr_time) { // allocate memory for arrived process(es)
    		mem.allocate(mode, aq.pop(), c, opt);
    	}
		//c.PrintTime();
		//std::cout<<"something happened:"<<std::endl;
    	//mem.print();	
    }
    opt.PrintEnd( mode , c.rtime() ) ;
    return 0;
}


