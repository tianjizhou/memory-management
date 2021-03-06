// =======================================
// Team members:
//
// Jinghua Feng 	fengj3	
// Jihui Nie		niej
// Tianji Zhou		zhout2
//
// github: https://github.com/tianjizhou/memory-management
// =======================================

// FILE: process_queue.h
#ifndef _process_queue_h
#define _process_queue_h

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "process.h"

class ProcessQueue {
public:
	// CONSTRUCTOR
	ProcessQueue() { next_time_ = -1; }
	// ACCESSORS
	int next_time() const { return next_time_; }
	bool empty() const { return processes_.empty(); }

	// MODIFIERS
	int load_file(const std::string& filename);
	void insert(int t, Process p);
	std::vector<Process> pop();	
	void extend(int t); // extend arrival times due to defragmentation

private:
	std::map<int, std::vector<Process> > processes_;
	int next_time_;

friend class Memory;
};

#endif
