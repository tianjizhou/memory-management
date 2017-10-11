// FILE: clock.h
#ifndef _clock_h
#define _clock_h

class Clock {
public:
	Clock() { time_ = 0; t_cs_ = 8;}
	int time() const { return time_; }
	void tick() { time_++; }
	void half_cs() {time_ += t_cs_/2 - 1;}

private:
	int time_;
	int t_cs_;
};

#endif