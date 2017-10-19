// =======================================
// Team members:
//
// Jinghua Feng 	fengj3	
// Jihui Nie		niej
// Tianji Zhou		zhout2
//
// github link: https://github.com/tianjizhou/operating-system/
// =======================================

// FILE: ready_queue.cpp
#include "ready_queue.h"

// Add new processes into ready queue without sorting
std::string ReadyQueue::push(std::vector<Process*> new_processes, const std::string& mode, 
                      Clock clk, const std::string& situation, Process* current_p) {
	std::stringstream output_str;
	for (unsigned int i = 0; i < new_processes.size(); i++) {

		new_processes[i]->arrive(clk.time());
		processes_.push_back(new_processes[i]);
		sort(mode);
        	clk.PrintTime(output_str) ;
        	output_str << "Process " << new_processes[ i ] -> ID() ;
        	if (mode == "SRT" && current_p &&new_processes[i]->tCPU() < current_p->tCPU()) {
	        	if ( situation == "arrival" ) 
	            		output_str << " arrived and will preempt " << current_p->ID() << " " ;
	        	else if ( situation == "IO" ) 
	            		output_str << " completed I/O and will preempt " << current_p->ID() << " " ;
	        	PrintPIDs(new_processes[i]->ID(),output_str);
        	}
        	else {
	        	if ( situation == "arrival" ) 
	            		output_str << " arrived and added to ready queue " ;
	        	else if ( situation == "IO" ) 
	            		output_str << " completed I/O; added to ready queue " ;
	        	PrintPIDs(output_str);        	
        	}
	}
            //std::cout << "Error: Invalid ready_queue push" << std::endl ;
	return output_str.str();
}

/*
void ReadyQueue::PrintPushProcesses( std::vector<Process*> proc_vec, Clock clk , const std::string & situation ) {
    std::vector<Process*>::iterator itr;
	for (itr = proc_vec.begin(); itr != proc_vec.end(); itr++) {
        clk.PrintTime() ;
        std::cout << "Process " << (*itr) -> ID() ;
        if ( situation == "arrival" ) 
            std::cout << " arrived and added to ready queue " ;
        else if ( situation == "IO" ) 
            std::cout << " completed I/O; added to ready queue " ;
        else
            std::cout << "Error: Invalid ready_queue push" << std::endl ;
        PrintPIDs() ;
	}
}
*/

// Process is preempted and added to ready queue
void ReadyQueue::push(Process* p, const std::string& mode, int t, int half_cs) {
	if (p != NULL) {
		p->arrive(t);
		processes_.push_back(p);
        if ( mode == "RR" ) {
            wait_time_ -= half_cs ;
        }
		sort(mode);
	}	
}

Process* ReadyQueue::pop() {
	Process* tmp = processes_.front();
	processes_.pop_front();
	return tmp;
}

// Sort the processes in the ready queue based on different algorithms
void ReadyQueue::sort(const std::string& mode) {
	if (mode == "FCFS")
		processes_.sort(FCFS);
	else if (mode == "SRT")
		processes_.sort(SRT);
	else if (mode == "RR")
		processes_.sort(RR);
	else
		std::cout << "Invalid algorithm" << std::endl;
}

void ReadyQueue::PrintPIDs() const {
    std::cout << "[Q" ;
    if( processes_.empty() ) std::cout << " <empty>" ;
    else {
        std::list<Process*>::const_iterator it ;
        for ( it = processes_.begin(); it != processes_.end(); ++it )
            std::cout << " " << ( *it ) -> ID() ;
    }
    std::cout << "]" << std::endl ;
}

void ReadyQueue::PrintPIDs(std::stringstream & output_str) const {
    output_str << "[Q" ;
    if( processes_.empty() ) output_str << " <empty>" ;
    else {
        std::list<Process*>::const_iterator it ;
        for ( it = processes_.begin(); it != processes_.end(); ++it )
            output_str << " " << ( *it ) -> ID() ;
    }
    output_str << "]" << std::endl ;
}

void ReadyQueue::PrintPIDs(char id, std::stringstream & output_str) const {
    output_str << "[Q" ;
    int count = 0;
    std::list<Process*>::const_iterator it ;
    for ( it = processes_.begin(); it != processes_.end(); ++it ) {
    	if ((*it)->ID() != id) {
    		output_str << " " << ( *it ) -> ID();
    		count++;
    	}        	
    }         	
    if(count == 0)
    	output_str << " <empty>" ;    
    output_str << "]" << std::endl ;
}

void ReadyQueue::tick() {
	wait_time_ += processes_.size();
}

// Sort algorithms
bool FCFS(Process* p1, Process* p2) {
	return p1->arrival_time() < p2->arrival_time()
		|| (p1->arrival_time() == p2->arrival_time() && p1->ID() < p2->ID());
}

bool SRT(Process* p1, Process* p2) {
	return p1->tCPU() < p2->tCPU()
		|| (p1->tCPU() == p2->tCPU() && p1->ID() < p2->ID());
}

bool RR(Process* p1, Process* p2) {
	return p1->arrival_time() < p2->arrival_time()
		|| (p1->arrival_time() == p2->arrival_time() && p1->ID() < p2->ID());
}
