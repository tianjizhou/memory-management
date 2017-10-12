// FILE: ready_queue.cpp
#include "ready_queue.h"

// Add new processes into ready queue without sorting
void ReadyQueue::push(std::vector<Process*> new_processes) {
	for (int i = 0; i < new_processes.size(); i++) {
		processes_.push_back(new_processes[i]);
	}
}

// Sort the processes in the ready queue based on different algorithms
void ReadyQueue::sort(const std::string& mode) {
	if (mode == "FCFS")
		processes_.sort(FCFS);
	else if (mode == "SRT")
		processes_.sort(SRT);
	else if (mode == "RR")
		processes_.sort(RR);
	else
		std::cout << "Invalid algorithm" << std::endl;
}

// Sort algorithms
bool FCFS(Process* p1, Process* p2) {
	return p1->arrival() < p2->arrival()
		|| (p1->arrival() == p2->arrival() && p1->ID() < p2->ID());
}

bool SRT(Process* p1, Process* p2) {
	return p1->tCPU() < p2->tCPU()
		|| (p1->tCPU() == p2->tCPU() && p1->ID() < p2->ID());
}

bool RR(Process* p1, Process* p2) {
	return p1->arrival() < p2->arrival()
		|| (p1->arrival() == p2->arrival() && p1->ID() < p2->ID());
}
