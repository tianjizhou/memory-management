// FILE: clock.h
#ifndef _clock_h
#define _clock_h

class Clock {
public:
	Clock() { time_ = 0;wait_time_=0;}
	int time() const { return time_; }
	void tick() { time_++; }
	void set(int t) {time_ = t; }
	void wait(int t) {wait_time_+=t;}
    int rtime() const { return time_ + wait_time_ ;}

    void PrintTime() { std::cout << "time " << time_+wait_time_ << "ms: " ; }

private:
	int time_;
	int wait_time_;
    int real_time_ ;
};

#endif
