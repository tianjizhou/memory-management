// =======================================
// Team members:
//
// Jinghua Feng 	fengj3	
// Jihui Nie		niej
// Tianji Zhou		zhout2
//
// github: https://github.com/tianjizhou/memory-management
// =======================================

// FILE: memory.h
#ifndef _memory_h
#define _memory_h

#include <string>
#include <iostream>
#include <set>

#include "process_queue.h"
#include "clock.h"
#include "output.h"


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
	void allocate(const std::string& mode, std::vector<Process> ps, Clock & c, Output & opt); 
	void allocate(const std::string& mode, Process p, Clock & c, Output & opt);
	void pop( const std::string& mode , Output & opt , Clock & c );


private:
	// helper functions
	int scan(const std::string& mode, int frame); // contiguous algorithms
	void defrag( Clock & c , Output & opt );
	//void insert(int index, Process p, int arr_time);
	void insert(int index, Process p, int arr_time, Output & opt , Clock & c);


	// representations
	std::string mem_;
	ProcessQueue pq_;
	std::map<char, std::vector<int> > page_table; 

	int total_idle_;
	std::size_t next_frame_;

	// constant parameters
	int num_frames_;
	int t_memmove_;
	int line_width_;
};

#endif
