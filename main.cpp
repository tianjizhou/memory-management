// FILE: main.cpp
#include <iostream>
#include <cstdlib>
#include <queue>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>  // std::setw
#include <ctype.h>

#include "process.h"
#include "cpu.h"
#include "ready_queue.h"
#include "clock.h"
#include "io.h"
#include "input.h"

int algorithm(std::map<int, std::vector<Process*> > initial_queue,
			  const std::string& mode, std::ofstream& out_str, int num_bursts, int burst_t);
void queue_reset(std::map<int, std::vector<Process*> > pending_queue);
void print_stat(std::ofstream& out_str, const std::string& mode, int num_bursts,
			    int burst_t, int wait_time, int half_t_cs, double num_cs, int num_preempts);

int main(int argc, char* argv[]) {
	if( argc != 3 ){
        	std::cerr << "ERROR: Invalid arguments\nUSAGE: ./a.out <input-file> <output-file>\n" ;
		return EXIT_FAILURE;
    }
	std::ofstream out_str(argv[2]);
	if (!out_str) {
		std::cerr << "Could not open " << argv[2] << " to write\n";
		return EXIT_FAILURE;
	}    

    char * file_name = argv[ 1 ] ;
    Input input ;
    ProcessVector process_vec ;
    ProcessMap initial_queue ;
    int num_bursts;
    int burst_t;

    input.ReadProcesses( file_name , process_vec, num_bursts, burst_t ) ;
    input.ConvertProcessVecToMap( process_vec , initial_queue ) ;

    //input.PrintProcessVector( process_vec ) ;
    //input.PrintProcessMap( initial_queue ) ;
	
	algorithm(initial_queue, "FCFS", out_str, num_bursts, burst_t);
	std::cout<<std::endl;
	algorithm(initial_queue, "SRT", out_str, num_bursts, burst_t);
	std::cout<<std::endl;
	algorithm(initial_queue, "RR", out_str, num_bursts, burst_t);

	return EXIT_SUCCESS;
}

int algorithm(std::map<int, std::vector<Process*> > initial_queue,
			  const std::string& mode, std::ofstream& out_str, int num_bursts, int burst_t) {

	std::map<int, std::vector<Process*> > pending_queue(initial_queue) ;
	queue_reset(pending_queue);

	ReadyQueue ready_queue;
	CPU core;
	Clock c;
	IO IOblocker;

    // Print start of simulation
    c.PrintTime() ;
    std::cout << "Simulator started for "<< mode << " " ;
    ready_queue.PrintPIDs() ;

	while (!(ready_queue.empty() && core.idle() && pending_queue.empty() && IOblocker.empty())
		|| core.cs_block()) {

// =========== TIME: starting moment of the Nth second, N = 0, 1, 2, ... =========

		// READY QUEUE UPDATE ------------------------------------

		// New processes arrive ----------------------------------	
		// NOTE: What if it preempts current process? Should not add to queue and should print out differently.
		// -------------------------------------------------------
		if ( pending_queue.find( c.time() ) != pending_queue.end()) {
            // print out new arrvial of processes when pushed in ready_queue
			ready_queue.push( pending_queue[ c.time() ] , mode, c,
							  "arrival", core.current_process());
			pending_queue.erase(c.time());
		}

		// Preemption --------------------------------------------	
		// NOTE: Not considering preemtion DURING content switching !!
		// -------------------------------------------------------
		if (!core.cs_block()) {// Not during context switch
			// Preempt process (SRT)
			if (!core.idle() && mode == "SRT" && !ready_queue.empty()
				&& ready_queue.front()->tCPU() < core.current_process()->tCPU()) {
				ready_queue.push(core.pop( c , ready_queue , mode), mode, c.time());
			}
	
		}

		// Load process into CPU ---------------------------------
        Process * proc_tmp ;
		if (core.idle() && !ready_queue.empty()) {
            proc_tmp = ready_queue.pop() ;
            // Print starting of CPU use 
			core.push( proc_tmp );
		}
		
	

// =========== TIME: Ns to (N+1)s ===============================================

		// CPU UPDATE --------------------------------------------
		if (!core.idle() && !core.cs_block()) {	// Not during context switch
			core.run(c, ready_queue);							// NOTE: If you prefer, we can integrate the conditions into the CPU.run() function.
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
		// CPU->Ready_queue (Preemption)
		//
		// I/O->ready_queue
		//
		// ========================================================

		// CLOCK UPDATE -----------------------------------------
		// time increases by 1 ms
		c.tick();
		ready_queue.tick(); // add wait time

		// CPU UPDATE -----------------------------------------
		// reduce remaining switching time and set/reset block
		core.tick();


		if (!core.cs_block()) {// Not during context switch
			// Remove completed process from CPU
			if (!core.idle() && core.single_complete()) {	
				IOblocker.push(core.pop( c , ready_queue , mode)); // print finish using CPU
			}
			// Preempt process (RR)
			else if (!core.idle() && mode == "RR" && !ready_queue.empty()
				&& core.slice_over()) {
				ready_queue.push(core.pop( c , ready_queue , mode), mode, c.time());
			}
			else if (!core.idle() && mode == "RR" && ready_queue.empty()
				&& core.slice_over()) {
				core.reset_slice(c);
			}
			// SRT moved to the start of this second, immediately after the arrival of new processes
	
		}

        // Print processes finishing I/O when pushed in ready_queue
		ready_queue.push( IOblocker.pop(), mode, c,
						  "IO" , core.current_process());

// =========== TIME: ending moment of the Xth second, i.e. start of (N+1)s ======
	}
    c.PrintTime() ;
    std::cout << "Simulator ended for " << mode << std::endl;
    print_stat(out_str, mode, num_bursts, burst_t,
    		   ready_queue.wait_time(), core.get_half_cs(), core.num_cs(), core.num_preempts());
	return EXIT_SUCCESS ;
}

void queue_reset(std::map<int, std::vector<Process*> > pending_queue){
	ProcessMap::iterator itr ;
    ProcessVector::iterator iter ;
    for ( itr = pending_queue.begin(); itr != pending_queue.end(); itr++ )
        for(iter = (itr->second).begin(); iter != (itr->second).end(); iter++)
				(*iter)->reset();

}

void print_stat(std::ofstream& out_str, const std::string& mode, int num_bursts,
			    int burst_t, int wait_time, int half_t_cs, double num_cs, int num_preempts) {
	out_str << std::setprecision(2) << std::fixed;
	out_str << "Algorithm " << mode << std::endl;
	out_str << "-- average CPU burst time: " << (double)burst_t/num_bursts << " ms" << std::endl;
	out_str << "-- average wait time: " << (double)wait_time/num_bursts << " ms" << std::endl;
	out_str << "-- average turnaround time: " << (double)(wait_time+burst_t+half_t_cs*2*num_cs)/num_bursts << " ms" << std::endl;
	out_str << std::setprecision(0) << std::fixed;
	out_str << "-- total number of context switches: " << num_cs << std::endl;
	out_str << "-- total number of preemptions: " << num_preempts << std::endl;
}
