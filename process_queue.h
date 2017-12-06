// FILE: process_queue.h
#ifndef _process_queue_h
#define _process_queue_h

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

#include "process.h"

class ProcessQueue {
public:
	// CONSTRUCTOR

	// ACCESSORS
	int next_time() const { return next_time_; }
	bool empty() const { return processes_.empty(); }

	// MODIFIERS
	int load_file(const std::string& filename);
	void insert(int t, Process p);
	std::set<Process> pop();	
	void extend(int t); // extend arrival times due to defragmentation

private:
	std::map<int, std::set<Process> > processes_;
	int next_time_;
};

#endif
