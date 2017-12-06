// FILE: memory.h
#ifndef _memory_h
#define _memory_h

#include <string>
#include <iostream>
#include <set>

#include "process_queue.h"
#include "clock.h"


class Memory {
public:
	// CONSTRUCTOR
	Memory();

	// ACCESSORS
	void print();
	int next_time() { return pq_.next_time(); }
	int next_frame() { return next_frame_; }
	bool empty() { return pq_.empty(); }

	// MODIFIERS
	void allocate(const std::string& mode, std::vector<Process> ps, Clock c); 
	void allocate(const std::string& mode, Process p, Clock c);
	void pop();


private:
	// helper functions
	int scan(const std::string& mode, int frame); // contiguous algorithms
	void defrag();
	void insert(int index, Process p, int arr_time);


	// representations
	std::string mem_;
	ProcessQueue pq_;

	int total_idle_;
	std::size_t next_frame_;

	// constant parameters
	int num_frames_;
	int t_memmove_;
	int line_width_;
};

#endif