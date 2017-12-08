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
	else if (mode == "best") {
		int best_index = -1;
		int best_size = total_idle_ + 1;
		std::size_t tail = 0; // end of last free partition
		// use loop to find best-fit partition
		while ((found = mem_.find(space, tail)) != std::string::npos) {
			tail = found + frame;
			while (mem_[tail] == '.') {
				tail++;
			}
			if ((int)(tail - found) < best_size) {
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
void Memory::insert(int index, Process p, int arr_time, Output & opt , Clock & c) {
    //opt.PrintArrive( arr_time , p.ID() , p.frame() ) ;
	p.set_index(index);
	pq_.insert(arr_time + p.run_time(), p);
	for (int i = 0; i < p.frame(); i++) {
			mem_[(index + i)%num_frames_] = p.ID(); // Memory is treated as a loop. 
													// All following functions are implemented based on the same assumption.
													// If it is wrong, however, only this function needs to be revised.

	}
	total_idle_ -= p.frame();
	next_frame_ = (index + p.frame())%num_frames_;

    opt.PrintPlace( c.rtime() , p.ID() ) ;
    print() ;
}

// memory allocation
void Memory::allocate(const std::string& mode, std::vector<Process> ps, Clock & c, Output & opt) {
	std::vector<Process>::iterator itr = ps.begin();
	for (; itr != ps.end(); itr++) {
		allocate(mode, *itr, c, opt );
	}
}



// return 0 when succeed; return -1 otherwise (no need to print memory)
void Memory::allocate(const std::string& mode, Process p, Clock & c, Output & opt) {
	if (mode == "non") { // non-contiguous memory management
        opt.PrintArrive( c.time() , p.ID() , p.frame() ) ;
		int required_frame = p.frame();
		if (total_idle_ < required_frame){ // not enough memory...skip
			// skip
			return;
		}
		int i;
		std::vector<int> page_list;
		while (required_frame>0) {			// allocate frames
			i=mem_.find('.');
			mem_[i]=p.ID();
			page_list.push_back(i);
			required_frame--;
		}
		pq_.insert(c.time() + p.run_time(), p);	// add this process to the quese so that its finishing time is watched
		total_idle_ -= p.frame();
		page_table[p.ID()]=page_list;		// add page list of this process to the overall page table
        opt.PrintPlace( c.rtime() , p.ID() ) ;
        print() ; // print memory
        opt.PrintPageTable( page_table ) ;
		
	}
	else { // contiguous memory management
        opt.PrintArrive( c.time() , p.ID() , p.frame() ) ;
		int index = scan(mode, p.frame());
		if (index == -2) { // not enough memory...skip
			// skip
            opt.PrintDefragBegin( c.rtime() , p.ID() , -1 ) ;
			return;
		}
		if (index == -1) { // degragmentation is required
            opt.PrintDefragBegin( c.rtime() , p.ID() , 1 ) ;
			defrag( c , opt );
            print() ;
			index = scan(mode, p.frame());
		}
		insert( index, p, c.time(), opt , c );
	}
}

void Memory::pop( const std::string & mode , Output & opt , Clock & c ) {
	std::vector<Process> top = pq_.pop();
	std::vector<Process>::iterator itr = top.begin();
	for (; itr != top.end(); itr++) {
		if (mode == "non") {	// non-contiguous
			std::vector<int>:: iterator it_vec = page_table[itr->ID()].begin();
			for (;it_vec!=page_table[itr->ID()].end();++it_vec)
				mem_[*it_vec]='.';			// clean memory
			page_table.erase(itr->ID());	// delete page list
		}
		else {	// contiguous
			int i=itr->index();
			for (;i<itr->index()+itr->frame();i++)
				mem_[i%num_frames_]='.';	// clean memory
		}
		total_idle_ += itr->frame();		// increase # of idle frames
        opt.PrintRemove( c.rtime() , itr->ID() ) ;
        print() ;
	}
}

void Memory::defrag( Clock & c , Output & opt ) {

	// For output purpose
	int frame_counter=0; // count the number of frames moved
	std::vector<char> frags; // store the names of moved processes

	// mark the start of the first hole(a piece of available memory sandwiched between processes)
	int hole_head=0;
	for (int i=0;i<num_frames_;i++)
		if (mem_[i]=='.')
		{
			hole_head=i;
			break;
		}
	
	// mark the next process(fragment) in memory after the first hole
	int next_frag=-1;

	// iterators to scan through all processes in memory
	std::map<int, std::vector<Process> > :: iterator it_map;
	std::vector<Process> :: iterator it_vec;

	while(1)
	{
		
		next_frag=-1; // suppose there's no fragment left
		for (int i=hole_head;i<num_frames_;i++)
			if (mem_[i]!='.')
			{
				next_frag=i;	// found a fragment
				break;
			}
		if (next_frag==-1) { 
            //opt.PrintDefragBegin( c.rtime(), p.ID() , -1 ) ;
            break; // if no fragment left
        }
		
		// find this process by scanning through all processes
		for (it_map=pq_.processes_.begin();it_map!=pq_.processes_.end();++it_map)
			for (it_vec=(it_map->second).begin();it_vec!=(it_map->second).end();++it_vec)
				if (it_vec->index()==next_frag)
					goto endloop;	// break out of nested loops

endloop:
		frame_counter+=it_vec->frame();
		frags.push_back(it_vec->ID());
		for (int i=next_frag;i<next_frag+(it_vec->frame());i++)
			mem_[i]='.';
		for (int i=hole_head;i<hole_head+(it_vec->frame());i++)
			mem_[i]=it_vec->ID();
		it_vec->set_index(hole_head);
		hole_head+=it_vec->frame();
		
	}

	c.wait(frame_counter*t_memmove_);
    opt.PrintDefragEnd( c.rtime() , frags , frame_counter  ) ;
    /*
	c.PrintTime();
	std::cout<<"defragmentaion starts"<<std::endl;
	c.wait(frame_counter*t_memmove_);
	c.PrintTime();
	std::cout<<"defragmentaion completed, moved"<<frame_counter<<" frames, processes:";
	// use vector<char> frags here to print the processes
	std::cout<<std::endl;
    */
	

}

void Memory::print() {
	std::string boundary(line_width_, '=');
	std::cout << boundary << std::endl;
	for (int i = 0; i < num_frames_/line_width_; i++) {
		std::cout << mem_.substr(i*line_width_, line_width_) << std::endl;
	}
	std::cout << boundary << std::endl;
}
