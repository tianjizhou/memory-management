// =======================================
// Team members:
//
// Jinghua Feng 	fengj3	
// Jihui Nie		niej
// Tianji Zhou		zhout2
//
// github link: https://github.com/tianjizhou/operating-system/
// =======================================

// FILE: cpu.h
#ifndef _cpu_h
#define _cpu_h

#include <iostream>
#include <stddef.h>
#include "process.h"
#include "ready_queue.h"
#include "clock.h"

class CPU {

	public:

		// CONSTRUCTOR
		CPU();

		// ACCESSORS
		bool idle() const { return process_ == NULL; }
		char current_processID() const { return process_->ID(); }
		Process* current_process() const {return process_; }
		bool single_complete() const { return process_->single_complete(); }
		bool complete() const { return process_->complete(); }
		bool slice_over() const { return remaining_t_slice_ <= 0; }
		bool cs_block() const { return cs_block_; }
        int get_half_cs() const { return t_cs_ / 2 ; }
        double num_cs() const { return num_cs_; }
        int num_preempts() const { return num_preempts_; }
        bool process_is_pushed() const { return process_pushed_ ; }

		// MODIFIER
		void unload();
		Process* pop( Clock clk , ReadyQueue ready_queue , const std::string& mode);
		void push(Process* p);
		void run( Clock clk , ReadyQueue ready_queue );
		void half_cs();
		void tick();
		void reset_slice(Clock clk);
        void set_process_status( bool st ) { process_pushed_ = st ; }

	private:

		// REPRESENTATION
		// CONSTANT
		int t_cs_;
		int t_slice_; // time slice for RR algorithm

		// UPDATING...
		Process* process_;
		int remaining_t_slice_;
		int remaining_t_cs_;
		bool cs_block_;
		bool cs_unblock_;
        bool process_pushed_ ;

		// STAT
		double num_cs_;	
		int num_preempts_;	
};

#endif
