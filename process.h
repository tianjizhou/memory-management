// =======================================
// Team members:
//
// Jinghua Feng 	fengj3	
// Jihui Nie		niej
// Tianji Zhou		zhout2
//
// github: https://github.com/tianjizhou/memory-management
// =======================================

// FILE: process.h
#ifndef _process_h
#define _process_h

class Process {
public:
	// CONSTRUCTOR
	Process();
	Process(char ID, int frame, int run_time) {
		ID_ = ID;
		frame_ = frame;
		run_time_ = run_time;
		index_ = -1;
	}

	// ACCESSORS
	char ID() const { return ID_; }
	int frame() const { return frame_; }
	int run_time() const { return run_time_; }
	int index() const { return index_; }

	// MODIFIERS
	void set_index(int index) { index_ = index; }

private:
	char ID_;
	int frame_;
	int run_time_;
	int index_; // staring point in memory
};

//bool operator< (const Process& left, const Process& right) {
//    return left.ID() < right.ID();
//}

#endif
