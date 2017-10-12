// FILE: cpu.cpp
#include "cpu.h"

CPU::CPU() {
	process_ = NULL;
	t_cs_ = 8;
	t_slice_ = 70;
	num_cs_ = 0;
	remaining_t_slice_ = t_slice_;
	cs_block_ = false;
	remaining_t_cs_ = 0;
}

void CPU::pop() {
	process_ = NULL;
	half_cs();
}

void CPU::push(Process* p) {
	process_ = p;
	process_->decrease_bursts();
	remaining_t_slice_ = t_slice_;
	half_cs();
}

void CPU::run() {
	if (process_ != NULL)
		process_->CPU_tick();
	remaining_t_slice_--;
}

void CPU::half_cs() {
	num_cs_ += 0.5;
	cs_block_ = true;
	remaining_t_cs_ = t_cs_/2;
}

void CPU::tick() {
	if (cs_block_)
		remaining_t_cs_--;
	if (remaining_t_cs_ == 0)
		cs_block_ = false;
}
