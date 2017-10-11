// FILE: cpu.h
#ifndef _cpu_h
#define _cpu_h

#include "process.h"

// For now it is assumed that there is not content switching time.
class CPU {

	public:

		// CONSTRUCTOR
		CPU() { process_ = NULL; t_slice_ = 70; num_cs_ = 0;}

		// ACCESSORS
		//int time() const { return clock_time_; }
		bool idle() const { return process_ == NULL; }
		char current_processID() const { return process_->ID(); }
		bool process_complete() const { return process_->complete(); }

		// MODIFIER
		//void tick() { clock_time_++; }
		void pop() { process_ = NULL; }
		void push(Process* p) { process_ = p; }
		void run() { if (process_ != NULL) process_->CPU_tick(); }
		void half_cs() { num_cs_ += 0.5; }

	private:

		// REPRESENTATION
		//int clock_time_;
		Process* process_;
		double num_cs_;
		int t_slice_; // time slice for RR algorithm


};

#endif
