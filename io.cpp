// FILE: io.cpp
#include "io.h"
#include <iostream>
IO::IO() {
	
};

std::string IO::PIDs() {
	std::string IDs; 
	for (std::list<Process*>::iterator it=processes_.begin();it!=processes_.end();it++) {
		IDs.append(1,(*it)->ID());
	}
	return IDs;
};

void IO::push(Process *p) {
	if (p != NULL) {
		if (!(p->IO_complete()))	// in the case IO time==0, do not push. Note: need to record finishing time!!!
			processes_.push_back(p);		
	}
};

std::vector<Process *> IO::pop() {
	std::vector<Process*> pop_list;		// store fully completed processes and pop them
	std::vector<Process*> ready_list;	// store not yet completed processes and return them (to ready_queue)
	std::list<Process*>::iterator it;
	std::vector<Process*>::iterator it_pop;
	std::vector<Process*>::iterator it_ready;

	for (it = processes_.begin(); it != processes_.end(); it++) {
		if ((*it)->complete())
				pop_list.push_back(*it);
		else if ((*it)->IO_complete())
				ready_list.push_back(*it);
	}

	for (it_ready = ready_list.begin(); it_ready != ready_list.end(); it_ready++) {	// return them to the ready_queue
		processes_.remove(*it_ready);
		(*it_ready)->IO_reset();
	}

	for (it_pop = pop_list.begin();it_pop != pop_list.end(); it_pop++) {			// pop completed processes
		processes_.remove(*it_pop);
	}
	return ready_list;
};

void IO::run() {
	for (std::list<Process*>::iterator it=processes_.begin();it!=processes_.end();it++) {
		(*it)->IO_tick();
	}
}
