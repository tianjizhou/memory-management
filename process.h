// FILE: process.h

#ifndef _process_h
#define _process_h

// For now there is only one CPU burst per process with no I/O burst required.
class Process {

  	public:

		// CONSTRUCTOR
		//Process(char c, int t) : PID_(c), single_CPU_time_(t), remaining_time_(t) {}
        Process( char c , int t_arrival , int t_burst , int c_burst , int t_IO ) {
                 PID_ = c;
                 arrival_time_ = t_arrival; 
                 single_CPU_time_ = t_burst;
                 remaining_time_ = t_burst;
                 remaining_bursts_ = c_burst; 
                 IO_time_ = t_IO;
        }

		// ACCESSORS
		char ID() const { return PID_; }
        int arrival() const { return arrival_time_ ; }
        int burst_time() const { return single_CPU_time_ ;}
        int burst_num() const { return remaining_bursts_ ; }
		int tIO() const { return IO_time_ ; }
		int tCPU() const { return remaining_time_; }
		bool single_complete() const { return (remaining_time_ == 0); }
		bool complete() const { return (remaining_time_ == 0 && remaining_bursts_ == 0); }
		// bool all_complete() {}

		// MODIFIERS
		void CPU_tick() { if (remaining_time_>0) remaining_time_--; }
        void decrease_bursts() { if ( remaining_bursts_ > 0 ) remaining_bursts_--; }

	private:

		// REPRESENTATION
		// CONSTANT
		char PID_;              // process id
		int single_CPU_time_;   // CPU burst time
		int IO_time_;           // IO wait time

		// UPDATING...
        int arrival_time_;      // arrival time
	    int remaining_bursts_;  // number of CPU bursts	
  		int remaining_time_;	// remaining time in single burst
  		//int total_CPU_time_;
		// int total_bursts_;
};

#endif
