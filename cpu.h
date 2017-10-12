// FILE: cpu.h
#ifndef _cpu_h
#define _cpu_h

#include "process.h"

class CPU {

	public:

		// CONSTRUCTOR
		CPU() { process_ = NULL;
				t_cs_ = 8;
				t_slice_ = 70;
				num_cs_ = 0;
				remaining_t_slice_ = t_slice_;
				cs_block_ = false;
				remaining_t_cs_ = 0;}

		// ACCESSORS
		bool idle() const { return process_ == NULL; }
		char current_processID() const { return process_->ID(); }
		Process* current_process() const {return process_; }
		bool single_complete() const { return process_->single_complete(); }
		bool complete() const { return process_->complete(); }
		bool slice_over() const { return remaining_t_slice_ == 0; }
		bool cs_block() const { return cs_block_; }

		// MODIFIER
		void pop() { process_ = NULL; }
		void push(Process* p) { process_ = p; process_->decrease_bursts(); remaining_t_slice_ = t_slice_; }
		void run() { if (process_ != NULL) process_->CPU_tick(); remaining_t_slice_--; }
		void half_cs() { num_cs_ += 0.5; cs_block_ = true; remaining_t_cs_ = t_cs_/2; }
		void tick() { if (cs_block_) remaining_t_cs_--; if (remaining_t_cs_ == 0) cs_block_ = false; }

	private:

		// REPRESENTATION
		// CONSTANT
		int t_cs_;
		int t_slice_; // time slice for RR algorithm


		// UPDATING...
		Process* process_;
		double num_cs_;
		int remaining_t_slice_;
		int remaining_t_cs_;
		bool cs_block_;
		


};

#endif
