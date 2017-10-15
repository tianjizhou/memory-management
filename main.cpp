#include <iostream>
#include <cstdlib>
#include <queue>
#include <map>
#include <vector>

#include <fstream>
#include <string>
#include <utility>  // std::pair, std::make_pair
#include <iomanip>  // std::setw
#include <ctype.h>

#include "process.h"
#include "cpu.h"
#include "ready_queue.h"
#include "clock.h"
#include "io.h"
#include "input.h"

int main(int argc, char* argv[]) {
	
	//
	if( argv[ 1 ] == NULL ){
        	std::cerr << "ERROR: Invalid arguments\nUSAGE: ./a.out <input-file>\n" ;
		return EXIT_FAILURE ;
    } 

    char * file_name = argv[ 1 ] ;
    Input input ;
    ProcessVector process_vec ;
    ProcessMap initial_queue ;

    input.ReadProcesses( file_name , process_vec ) ;
    input.ConvertProcessVecToMap( process_vec , initial_queue ) ;

    //input.PrintProcessVector( process_vec ) ;
    //input.PrintProcessMap( initial_queue ) ;

	std::map<int, std::vector<Process*> > pending_queue;
	pending_queue = initial_queue ;

	ReadyQueue ready_queue;
	CPU core;
	Clock c;
	IO IOblocker;
	//std::string mode("FCFS");
	//std::string mode("RR");
	std::string mode("SRT");

    // Print start of simulation
    c.PrintTime() ;
    std::cout << "Simulator started for "<< mode << " " ;
    ready_queue.PrintPIDs() ;

	while (!(ready_queue.empty() && core.idle() && pending_queue.empty() && IOblocker.empty())
		|| core.cs_block()) {

// =========== TIME: starting moment of the Nth second, N = 0, 1, 2, ... =========

		// READY QUEUE UPDATE ------------------------------------
		// New processes arrive
		if ( pending_queue.find( c.time() ) != pending_queue.end()) {
            // print out new arrvial of processes when pushed in ready_queue
			ready_queue.push( pending_queue[ c.time() ] , mode, c.time(), c, "arrival");
			pending_queue.erase(c.time());
		}
		// Load process into CPU ---------------------------------
        Process * proc_tmp ;
		if (core.idle() && !ready_queue.empty()) {
            proc_tmp = ready_queue.pop() ;
            // Print starting of CPU use 
			core.push( proc_tmp );
            std::cout << "time " << c.time() + core.get_half_cs() << "ms: Process "
                      << core.current_processID() << " started using the CPU ";

            ready_queue.PrintPIDs() ;
		}	

// =========== TIME: Ns to (N+1)s ===============================================

		// CPU UPDATE --------------------------------------------
		if (!core.idle() && !core.cs_block()) {	// Not during context switch
			core.run();							// NOTE: If you prefer, we can integrate the conditions into the CPU.run() function.
		}

		// IO QUEUE UPDATE ---------------------------------------
		IOblocker.run();						// I/O always ticks, regardless of switching time


		// Print out what was done during this second.
		/*
        if (core.cs_block())
			std::cout << "time: " << c.time() << " || CPU: " <<" process " << core.current_processID() <<" switching";
		else if (!core.idle())
			std::cout << "time: " << c.time() << " || CPU: " <<" process " << core.current_processID() <<" running  ";
		else
			std::cout << "time: " << c.time() << " || CPU:  idle......         ";		
		std::cout <<" || IO: "<<IOblocker.PIDs()<<std::endl;
        */


// =========== TIME: ending moment of the Nth second, i.e. start of (N+1)s ======

		// ========================================================
		// CPU and I/O runs are completed, now switch(if needed).
		//
		// CPU->I/O			(Completion)
		// CPU->ready_queue (Preemption)
		//
		// I/O->ready_queue
		//
		// ========================================================

		// CLOCK UPDATE -----------------------------------------
		// time increases by 1 ms
		c.tick();

		// CPU UPDATE -----------------------------------------
		// reduce remaining switching time and set/reset block
		core.tick();


		if (!core.cs_block()) {// Not during context switch
			// Remove completed process from CPU		NOTE: Let I/O decide if it's a full completion. 
            // QUESTION: Why do we need to check the ready_queue? (was && !ready_queue.empty())
			if (!core.idle() && core.single_complete()) {	
				IOblocker.push(core.pop( c , ready_queue )); // print finish using CPU
			}
			// Preempt process (RR)
			else if (!core.idle() && mode == "RR" && !ready_queue.empty()
				&& core.slice_over()) {
				ready_queue.push(core.pop( c , ready_queue ), mode, c.time());
			}
			// Preempt process (SRT)
			else if (!core.idle() && mode == "SRT" && !ready_queue.empty()
				&& ready_queue.front()->tCPU() < core.current_process()->tCPU()) {
				ready_queue.push(core.pop( c , ready_queue ), mode, c.time());
			}
	
		}

        // Print processes finishing I/O when pushed in ready_queue
		ready_queue.push( IOblocker.pop(), mode, c.time(), c, "IO" );

// =========== TIME: ending moment of the Xth second, i.e. start of (N+1)s ======
	}
    c.PrintTime() ;
    std::cout << "Simulator ended for " << mode << std::endl;
	return EXIT_SUCCESS ;
}
