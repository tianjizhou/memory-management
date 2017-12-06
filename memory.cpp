// FILE: memory.cpp
#include "memory.h"

Memory::Memory() {
	num_frames_ = 256;
	t_memmove_ = 1;
	line_width_ = 32;

	mem_ = std::string(num_frames_,'.');
	total_idle_ = num_frames_;
	next_frame_ = 0;
}


// return the starting index of the available partition when found
// return -2 when memory is not enough
// return -1 when degragmentation is required
int Memory::scan(const std::string& mode, int frame) {
	if (total_idle_ < frame)
		return -2; // not enough memory

	std::string space(frame, '.');
	std::size_t found;
	if (mode == "next") {
		// re-order the map by starting beyond the end of
		// the most recently placed process
		std::string reordered_mem = mem_.substr(next_frame_)
									+ mem_.substr(0, next_frame_);
		found = reordered_mem.find(space);
		if (found != std::string::npos)
			return (int)(found + next_frame_);		
	}
	else if (mode == "first") {
		found = mem_.find(space);
		if (found != std::string::npos)
			return (int)found;
	}
	else if (mode == " best") {
		int best_index = -1;
		int best_size = total_idle_ + 1;
		std::size_t tail = 0; // end of last free partition
		// use loop to find best-fit partition
		while ((found = mem_.find(space, tail)) != std::string::npos) {
			tail = found + frame;
			while (mem_[tail] == '.') {
				tail++;
			}
			if (tail - found < best_size) {
				best_index = found;
				best_size = tail - found;
			}
		}
		// check the connection of bottom and top of memory
		int bottom_size = 0;
		int top_size = 0;
		while (mem_[num_frames_-1-bottom_size] == '.') {
			bottom_size++;
		}
		while (mem_[top_size] == '.') {
			top_size++;
		}
		int connection_size = bottom_size + top_size;
		if (bottom_size != 0 && connection_size >= frame
			&& connection_size < best_size)
			best_index = num_frames_-bottom_size;

		if (best_index != -1)
			return best_index;		
	}

	return -1; // need defragmentation
}


// add process starting from the specified index
void Memory::insert(int index, Process p, int arr_time) {
	p.set_index(index);
	pq_.insert(arr_time + p.run_time(), p);
	for (int i = 0; i < p.frame(); i++) {
			mem_[(index + i)%num_frames_] = p.ID();

	}
	total_idle_ -= p.frame();
	next_frame_ = index + p.frame();
}

// memory allocation
void Memory::allocate(const std::string& mode, std::set<Process> ps, Clock c) {
	std::set<Process>::iterator itr = ps.begin();
	for (; itr != ps.end(); itr++) {
		allocate(mode, *itr, c);
	}
}



// return 0 when succeed; return -1 otherwise (no need to print memory)
void Memory::allocate(const std::string& mode, Process p, Clock c) {
	if (mode == "non") { // non-contiguous memory management
		// hint: use mem_.find('.', position) in loop
	}
	else { // contiguous memory management
		int index = scan(mode, p.frame());
		if (index == -2) { // not enough memory...skip
			// skip
			
		}
		if (index == -1) { // degragmentation is required
			defrag();
			index = scan(mode, p.frame());
		}
		insert(index, p, c.time());
	}
}

void Memory::pop() {

}

void Memory::defrag() {

}

void Memory::print() {
	std::string boundary(line_width_, '=');
	std::cout << boundary << std::endl;
	for (int i = 0; i < num_frames_/line_width_; i++) {
		std::cout << mem_.substr(i*line_width_, line_width_) << std::endl;
	}
	std::cout << boundary << std::endl;
}
