// FILE: cpu.h
#include "process.h"

// For now it is assumed that there is not content switching time.
class CPU {

	public:

		// CONSTRUCTOR
		CPU() { clock_time_=0; process_=NULL; }

		// ACCESSORS
		int time() { return clock_time_; }
		bool idle() { return (process_==NULL); }
		char current_process() { return process_->ID(); }
		bool process_complete() { return process_->complete(); }

		// MODIFIER
		void tick() { clock_time_++; }
		void pop() { process_=NULL; }
		void push(Process * p) { process_=p; }
		void run() { if (process_!=NULL) process_->CPU_tick(); }



	private:

		// REPRESENTATION
		int clock_time_;
		Process * process_;
		// int content_switching_time_;


};
