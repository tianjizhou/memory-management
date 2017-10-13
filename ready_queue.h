// FILE: ready_queue.h
#ifndef _readyqueue_h
#define _readyqueue_h

#include "process.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>


class ReadyQueue {
public:
	// CONSTRUCTOR
	ReadyQueue(){ wait_time_ = 0; }
	
	// ACCESSORS
	Process* front() { return processes_.front(); }
	bool empty() const { return processes_.empty(); }
	int size() const { return processes_.size(); }
	
	// MODIFIERS
	void push(std::vector<Process*> new_processes, const std::string& mode, int t);
	void push(Process* process, const std::string& mode, int t);
	Process* pop();
	void sort(const std::string& mode);

private:
	// REPRESENTATION
	std::list<Process*> processes_;
	int wait_time_;
};

	bool FCFS(Process* p1, Process* p2);
	bool SRT(Process* p1, Process* p2);
	bool RR(Process* p1, Process* p2);

#endif