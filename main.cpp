#include <iostream>
#include <cstdlib>
#include <queue>

#include "process.h"
#include "cpu.h"



int main(int argc, char* argv[]) {


	// three processes arrive at time 0 and are put into ready queue immediately
	Process A('A',2);
	Process B('B',3);
	Process C('C',4);

	std::queue<Process *> ready_queue;

	ready_queue.push(&A);
	ready_queue.push(&B);
	ready_queue.push(&C);
	
	CPU core;

	while(!(ready_queue.size()==0&&core.idle())){
		if (core.idle()) {
			core.push(ready_queue.front());
			ready_queue.pop();
		}
		else if (core.process_complete()) {
			core.pop();
		}
		else {
			std::cout<<"time: "<<core.time()<<" process: "<<core.current_process()<<'\n';
			core.run();
			core.tick();
		}
	}
	
	return EXIT_SUCCESS;
}
