// FILE: ready_queue.h
#ifndef _readyqueue_h
#define _readyqueue_h

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "process.h"
#include "clock.h"

class ReadyQueue {
public:
	// CONSTRUCTOR
	ReadyQueue(){ wait_time_ = 0; }
	
	// ACCESSORS
	Process* front() { return processes_.front(); }
	bool empty() const { return processes_.empty(); }
	int size() const { return processes_.size(); }
	void PrintPIDs() const;
	void PrintPIDs(std::stringstream & output_str) const;
	void PrintPIDs(char id, std::stringstream & output_str) const;
	int wait_time() const { return wait_time_; }
	
	// MODIFIERS
	std::string push(std::vector<Process*> new_processes, const std::string& mode,
			  Clock clk, const std::string& situation, Process* current_p);
   // void PrintPushProcesses( std::vector<Process*> proc_vec, Clock clk , const std::string & situation ) ;
	void push(Process* process, const std::string& mode, int t, int half_cs);
	Process* pop();
	void sort(const std::string& mode);
	void tick();

    

private:
	// REPRESENTATION
	std::list<Process*> processes_;
	int wait_time_;
};

	bool FCFS(Process* p1, Process* p2);
	bool SRT(Process* p1, Process* p2);
	bool RR(Process* p1, Process* p2);

#endif
