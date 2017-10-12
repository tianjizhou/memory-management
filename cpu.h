// FILE: cpu.h
#ifndef _cpu_h
#define _cpu_h

#include "process.h"

class CPU {

	public:

		// CONSTRUCTOR
		CPU() { process_ = NULL; t_slice_ = 70; num_cs_ = 0; remaining_t_slice_ = t_slice_; }

		// ACCESSORS
		bool idle() const { return process_ == NULL; }
		char current_processID() const { return process_->ID(); }
		Process* current_process() const {return process_; }
		bool process_complete() const { return process_->complete(); }
		bool slice_over() const {return remaining_t_slice_ == 0; }

		// MODIFIER
		void pop() { process_ = NULL; }
		void push(Process* p) { process_ = p; remaining_t_slice_ = t_slice_; }
		void run() { if (process_ != NULL) process_->CPU_tick(); remaining_t_slice_--; }
		void half_cs() { num_cs_ += 0.5; }

	private:

		// REPRESENTATION
		// CONSTANT
		int t_slice_; // time slice for RR algorithm

		// UPDATING...
		Process* process_;
		double num_cs_;
		int remaining_t_slice_;
		


};

#endif
