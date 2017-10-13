// FILE: cpu.cpp
#include "cpu.h"

CPU::CPU() {
	process_ = NULL;
	t_cs_ = 8;
	t_slice_ = 8;
	num_cs_ = 0;
	remaining_t_slice_ = t_slice_;
	cs_block_ = false;
	cs_unblock_ = false;
	remaining_t_cs_ = 0;
}

Process* CPU::pop() {
	if (cs_unblock_) {
		Process* tmp = process_;
		process_ = NULL;
		cs_unblock_ = false;
		return tmp;
	}
	else {
		half_cs();
		return NULL;
	}	
}

void CPU::push(Process* p) {
	process_ = p;
	process_->decrease_bursts();
	remaining_t_slice_ = t_slice_;
	half_cs();
}

void CPU::run() {
	if (process_ != NULL && !cs_block_)
		process_->CPU_tick();
	remaining_t_slice_--;
	if (cs_unblock_ == true)
		cs_unblock_ = false;
}

void CPU::half_cs() {
	num_cs_ += 0.5;
	cs_block_ = true;
	remaining_t_cs_ = t_cs_/2;
}

void CPU::tick() {
	if (cs_block_)
		remaining_t_cs_--;
	if (remaining_t_cs_ == 0 && cs_block_ == true) {
		cs_block_ = false;
		cs_unblock_ = true;
	}
		
}
