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

Process* CPU::pop( Clock clk , ReadyQueue ready_queue ) {
	
	if (cs_unblock_) {

		if (process_->single_complete()) {
			if (!(process_->complete())) {
				process_->decrease_bursts();
				process_->CPU_reset();
			}
		}

		Process* tmp = process_;
		process_ = NULL;
		cs_unblock_ = false;
		return tmp;
	}
	else {
        int remaining_bursts = process_-> burst_num() ;
        if ( remaining_bursts > 0 ) {
            clk.PrintTime() ;
            std::cout << "Process " << process_ -> ID() <<" completed a CPU burst; " 
                      << remaining_bursts ; 
            if ( remaining_bursts == 1 )
                std::cout << " burst to go " ;
            else
                std::cout << " bursts to go " ;
            ready_queue.PrintPIDs() ;

            clk.PrintTime() ;
            std::cout << "Process " << process_ -> ID() << " switching out of CPU; will block on I/O until time "
                << clk.time() + get_half_cs() + process_ -> tIO() << "ms " ;
            ready_queue.PrintPIDs() ;
        }
        else {
            clk.PrintTime() ;
            std::cout << "Process " << process_ -> ID() <<" terminated " ;
            ready_queue.PrintPIDs() ;
        }

        half_cs();
        return NULL;
	}	
}

void CPU::push(Process* p) {
	process_ = p;
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
