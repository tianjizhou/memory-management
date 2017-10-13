// FILE: io.cpp
#include "io.h"
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
		p->IO_reset();
		if (!(p->IO_complete()))	// in the case IO time==0, do not push. Note: need to record finishing time!!!
			processes_.push_back(p);		
	}
};

std::vector<Process *> IO::pop() {
	std::vector<Process*> pop_list;
	std::vector<Process*> ready_list;
	std::list<Process*>::iterator it;
	std::vector<Process*>::iterator it2;

	for (it = processes_.begin(); it != processes_.end(); it++) {
		if ((*it)->IO_complete()) {
			if ((*it)->complete())
				pop_list.push_back(*it);
			else
				ready_list.push_back(*it);
		}
	}

	for (it2 = ready_list.begin(); it2 != ready_list.end(); it2++) {
		processes_.remove(*it2);
	}

	for (it2 = pop_list.begin();it2 != pop_list.end(); it2++) {
		processes_.remove(*it2);
	}

	return ready_list;
};

void IO::tick() {
	for (std::list<Process*>::iterator it=processes_.begin();it!=processes_.end();it++) {
		(*it)->IO_tick();
	}
}
