// FILE: io.cpp
#include "io.h"
#include <vector>
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
	p->IO_reset();
	processes_.push_back(p);
};

void IO::pop() {
	std::vector<Process*> pop_list;
	std::vector<Process*> ready_list;

	for (std::list<Process*>::iterator it=processes_.begin();it!=processes_.end();it++) {
		if ((*it)->IO_complete()) {
			if ((*it)->complete())
				ready_list.push_back((*it));
			else
				pop_list.push_back((*it));
		}
	}

	for (std::vector<Process*>::iterator it2=ready_list.begin();it2!=ready_list.end();it2++) {
		processes_.remove((*it2));
	}

	for (std::vector<Process*>::iterator it3=pop_list.begin();it3!=pop_list.end();it3++) {
		processes_.remove((*it3));
	}

	//add ready_list to ready queue
};

void IO::tick() {
	for (std::list<Process*>::iterator it=processes_.begin();it!=processes_.end();it++) {
		(*it)->IO_tick();
	}
	pop();
}
