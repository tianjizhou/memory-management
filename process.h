// FILE: process.h

#ifndef _process_h
#define _process_h

// For now there is only one CPU burst per process with no I/O burst required.
class Process {

  	public:

		// CONSTRUCTOR
		Process(char c, int t) : PID_(c), total_CPU_time_(t), remaining_time_(t) {}

		// ACCESSORS
		char ID() { return PID_; }
		int tCPU() { return remaining_time_; }
		// int tIO() {}
		bool complete() { return (remaining_time_ == 0); }
		// bool all_complete() {}

		// MODIFIERS
		void CPU_tick() { if (remaining_time_>0) remaining_time_--; }

	private:

		// REPRESENTATION
		char PID_;
  		int total_CPU_time_;
  		int remaining_time_;
		// int IO_time_;
		// int total_bursts_;
		// int remaining_bursts_;
};

#endif
