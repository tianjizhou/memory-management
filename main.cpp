#include <iostream>
#include <cstdlib>
#include <queue>
#include <map>
#include <vector>

#include "process.h"
#include "cpu.h"
#include "ready_queue.h"
#include "clock.h"
#include "io.h"



int main(int argc, char* argv[]) {

    // Load input and store all processes in pending queue
	// PID, arriving time, each CPU burst, # of bursts, I/O time
	Process A('A' , 1 , 10 , 1 , 5 );
    Process B('B' , 1 , 9 , 1 , 20 );
    Process C('C' , 1 , 8 , 1 , 5 );
	Process D('D' , 7 , 2 , 1 , 5 );
	std::vector<Process*> tmp1, tmp2;
	tmp1.push_back(&A);
	tmp1.push_back(&B);
	tmp1.push_back(&C);
	tmp2.push_back(&D);
	std::map<int, std::vector<Process*> > pending_queue;
	pending_queue[1] = tmp1;
	pending_queue[7] = tmp2;

	ReadyQueue ready_queue;
	CPU core;
	Clock c;
	IO IOblocker;
	std::string mode("SRT");
	bool new_arrival = false;

	// Not considering IO queue
	while (!(ready_queue.empty() && core.idle() && pending_queue.empty())
		|| core.cs_block()) {
		// READY QUEUE UPDATE ------------------------------------
		// New processes arrive
		if (pending_queue.find(c.time()) != pending_queue.end()) {
			ready_queue.push(pending_queue[c.time()]);
			pending_queue.erase(c.time());
			ready_queue.sort(mode);
			new_arrival = true;
		}
		// Processes finish IO

		// CPU UPDATE --------------------------------------------
		core.tick();
		// Not during context switch
		if (!core.cs_block()) {
			// Load process into CPU
			if (core.idle() && !ready_queue.empty()) {
				core.push(ready_queue.front());
				ready_queue.pop();
			}
			// Remove totally completed process from CPU
			else if (!core.idle() && core.complete()) {
				IOblocker.push(core.current_process());
				core.pop();
			}
			// Remove completed process from CPU
			else if (!core.idle() && core.single_complete() && !ready_queue.empty()) {
				IOblocker.push(core.current_process());
				core.pop();
			}
			// Preempt process (RR)
			else if (!core.idle() && mode == "RR" 
				&& core.slice_over() && !ready_queue.empty()) {
				core.pop(); // should add process into Ready queue
			}
			// Preempt process (SRT)
			else if (new_arrival && !core.idle() && mode == "SRT" 
				&& ready_queue.front()->tCPU() < core.current_process()->tCPU()) {
				core.pop(); // should add process into Ready queue
			}
			else {
				core.run();
			}
			new_arrival = false;		
		}

		// IO QUEUE UPDATE -----------------------------------------
		IOblocker.tick();		

		// CLOCK UPDATE -----------------------------------------
		// time increases by 1 ms
		if (core.cs_block())
			std::cout << "time: " << c.time() << " CPU: " <<" switching";
		else if (!core.idle())
			std::cout << "time: " << c.time() << " CPU: "<<" process: " << core.current_processID();
		else
			std::cout << "time: " << c.time() << " CPU:  idle ";		
		std::cout <<" IO: "<<IOblocker.PIDs()<<std::endl;
		c.tick();
	}
	
	return EXIT_SUCCESS;
}
