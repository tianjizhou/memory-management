// FILE: process.h

#ifndef _process_h
#define _process_h


class Process {

  	public:

		// CONSTRUCTOR
        Process( char c , int t_arrival , int t_burst , int c_burst , int t_IO ) {
                 PID_ = c;
                 initial_arrival_time_ = t_arrival; 
                 single_CPU_time_ = t_burst;
		 		 total_bursts_ = c_burst; 
                 IO_time_ = t_IO;
                 reset();
        }

		// ACCESSORS
        char ID() const { return PID_; }
        int arrival_time() const { return arrival_time_ ; }
        int initial_arrival_time() const { return initial_arrival_time_ ; }
        int burst_time() const { return single_CPU_time_ ;}
        int burst_num() const { return remaining_bursts_ ; }
        int tIO() const { return IO_time_ ; }
		int tCPU() const { return remaining_time_; }
		bool single_complete() const { return (remaining_time_ == 0); }
		bool complete() const { return (remaining_time_ == 0 && remaining_bursts_ == 0); }
		bool IO_complete() const { return (remaining_IO_time_ == 0); }

		// MODIFIERS
		void reset() { remaining_bursts_ = total_bursts_-1;
			remaining_time_ = single_CPU_time_;
			remaining_IO_time_ = IO_time_;
			arrival_time_ = initial_arrival_time_; }

		void CPU_tick() { if (remaining_time_>0) remaining_time_--; }
		void CPU_reset() { remaining_time_ = single_CPU_time_; }
		void IO_tick() { if (remaining_IO_time_>0) remaining_IO_time_--; }
		void IO_reset() { remaining_IO_time_ = IO_time_; }
        void decrease_bursts() { if ( remaining_bursts_ > 0 ) remaining_bursts_--; }
        void arrive(int t) { arrival_time_ = t; }

	private:

		// REPRESENTATION
		// CONSTANT
		char PID_; // process id
		int initial_arrival_time_; // arrival time
		int single_CPU_time_; // CPU burst time
		int total_bursts_;
		int IO_time_; // IO time

		// UPDATING...
	    int remaining_bursts_;  // number of CPU bursts	
  		int remaining_time_;	// remaining time in single burst
		int remaining_IO_time_; // remaining time of current I/O blocking
		int arrival_time_;
};

#endif
