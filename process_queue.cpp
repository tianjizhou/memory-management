// =======================================
// Team members:
//
// Jinghua Feng 	fengj3	
// Jihui Nie		niej
// Tianji Zhou		zhout2
//
// github: https://github.com/tianjizhou/memory-management
// =======================================

// FILE: process_queue.cpp
#include "process_queue.h"
#include <string.h>

// read from input file
int ProcessQueue::load_file(const std::string& filename) {
	// open input file
	char* filename_c = new char[filename.length()+1];
	strcpy(filename_c,filename.c_str());
	std::ifstream in_str(filename_c);
	delete[] filename_c;
    if (!in_str) {
        std::cout << "ERROR: failed to open file" << std::endl;
        return -1;
    }

    // parse file
    std::string line;
    char ID, delimiter;
    int frame;
    int arr_time, run_time;

    while (getline(in_str, line)) { // read one line
    	std::stringstream ss(line);
    	ss >> ID >> frame;
    	while (ss >> arr_time) { // read the arrival events in loop
    		ss  >> delimiter >> run_time;
    		Process p(ID, frame, run_time);
    		processes_[arr_time].push_back(p);
    	}
    }

    next_time_ = (processes_.begin())->first;
    return 0;
}

// insert single process
void ProcessQueue::insert(int t, Process p) {
    processes_[t].push_back(p);
    next_time_ = (processes_.begin())->first;
}

std::vector<Process> ProcessQueue::pop() {
	std::vector<Process> top = (processes_.begin())->second;
	processes_.erase(processes_.begin());
    if (processes_.empty())
        next_time_ = -1;
    else 
        next_time_ = (processes_.begin())->first;
	return top;
}

// extend arrival times due to defragmentation
void ProcessQueue::extend(int t) {
    std::map<int, std::vector<Process> > extended_processes_;
	std::map<int, std::vector<Process> >::iterator itr = processes_.begin();
	for (; itr != processes_.end(); itr++) {
        extended_processes_[itr->first + t] = itr->second;
	}
    processes_ = extended_processes_;
    next_time_ += t;
}

