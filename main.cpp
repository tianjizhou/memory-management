#include <iostream>
#include <cstdlib>
#include <queue>
#include <map>
#include <vector>

#include "process.h"
#include "cpu.h"
#include "ready_queue.h"
#include "clock.h"



int main(int argc, char* argv[]) {

    // Load input and store all processes in pending queue
	Process A('A' , 1 , 2 , 1 , 1 );
    Process B('B' , 1 , 3 , 1 , 1 );
    Process C('C' , 1 , 4 , 1 , 1 );
	std::map<int, std::vector<Process*> > pending_queue;
	std::vector<Process*> tmp;
	tmp.push_back(&A);
	tmp.push_back(&B);
	tmp.push_back(&C);
	pending_queue[1] = tmp;

	ReadyQueue ready_queue;
	CPU core;
	Clock c;

	// Not considering IO queue
	while (!(ready_queue.empty() && core.idle() && pending_queue.empty())) {
		// New processes arrive
		if (pending_queue.find(c.time()) != pending_queue.end()) {
			ready_queue.push(pending_queue[c.time()]);
			pending_queue.erase(c.time());
			//ready_queue.sort("SRT");
		}
		// Load process into CPU
		if (core.idle() && !ready_queue.empty()) {
			core.push(ready_queue.front());
			ready_queue.pop();
			core.half_cs(); // Load context
			c.half_cs();
		}
		// Remove process from CPU
		else if (!core.idle() && core.process_complete()) {
			core.pop(); // should add process into IO queue
			core.half_cs(); // Remove context
			c.half_cs();
		}
		
		if (!core.idle())
			std::cout << "time: " << c.time() <<" process: " << core.current_processID() << std::endl;
		else
			std::cout << "time: " << c.time() << std::endl;		
		core.run();
		c.tick();


	}
	
	return EXIT_SUCCESS;
}
