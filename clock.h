// =======================================
// Team members:
//
// Jinghua Feng 	fengj3	
// Jihui Nie		niej
// Tianji Zhou		zhout2
//
// github link: https://github.com/tianjizhou/operating-system/
// =======================================

// FILE: clock.h
#ifndef _clock_h
#define _clock_h

#include <sstream>

class Clock {
public:
	Clock() { time_ = 0;}
	int time() const { return time_; }
	void tick() { time_++; }

    void PrintTime() { std::cout << "time " << time_ << "ms: " ; }
    void PrintTime(std::stringstream & output_str) { output_str << "time " << time_ << "ms: " ; }
    void PrintTime( int t ) { std::cout << "time " << t << "ms: " ; }

private:
	int time_;
};

#endif
