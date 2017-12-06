// FILE: process_queue.cpp
#include "process_queue.h"

// read from input file
int ProcessQueue::load_file(const std::string& filename) {
	// open input file
	std::ifstream in_str(filename);
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
    		processes_[arr_time].insert(p);
    	}
    }

    next_time_ = (processes_.begin())->first;
    return 0;
}

// insert single process
void ProcessQueue::insert(int t, Process p) {
    processes_[t].insert(p);
    next_time_ = (processes_.begin())->first;
}

std::set<Process> ProcessQueue::pop() {
	std::set<Process> top = (processes_.begin())->second;
	processes_.erase(processes_.begin());
    if (processes_.empty())
        next_time_ = -1;
    else 
        next_time_ = (processes_.begin())->first;
	return top;
}

// extend arrival times due to defragmentation
void ProcessQueue::extend(int t) {
    std::map<int, std::set<Process> > extended_processes_;
	std::map<int, std::set<Process> >::iterator itr = processes_.begin();
	for (; itr != processes_.end(); itr++) {
        extended_processes_[itr->first + t] = itr->second;
	}
    processes_ = extended_processes_;
    next_time_ += t;
}

