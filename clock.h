// FILE: clock.h
#ifndef _clock_h
#define _clock_h

class Clock {
public:
	Clock() { time_ = 0;}
	int time() const { return time_; }
	void tick() { time_++; }

    void PrintTime() { std::cout << "time " << time_ << "ms: " ; }

private:
	int time_;
};

#endif
