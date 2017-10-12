#include <iostream>
#include <cstdlib>
#include <queue>
#include <map>
#include <vector>

#include <fstream>
#include <string>
#include <utility>  // std::pair, std::make_pair
#include <iomanip>  // std::setw
#include <ctype.h>

#include "process.h"
#include "cpu.h"
#include "ready_queue.h"
#include "clock.h"
#include "io.h"

typedef std::vector< Process * > ProcessVector ;
typedef std::map< int , std::vector< Process * > > ProcessMap ;

int CountCharacter( std::string str ) ;
int ReadProcesses( const char * file_name , ProcessVector & process_vec ) ;
void PrintProcessVector( const ProcessVector & process_vec ) ;
void ConvertProcessVecToMap( const ProcessVector & process_vec , ProcessMap & initial_queue ) ;
void PrintProcessMap( const ProcessMap & initial_queue ) ;

int main(int argc, char* argv[]) {
	
	//
	if( argv[ 1 ] == NULL ){
        	std::cerr << "ERROR: Invalid arguments\nUSAGE: ./a.out <input-file>\n" ;
		return EXIT_FAILURE ;
	    } 

	    char * file_name = argv[ 1 ] ;
	    ProcessVector process_vec ;
	    ProcessMap initial_queue ;

	    ReadProcesses( file_name , process_vec ) ;
	    ConvertProcessVecToMap( process_vec , initial_queue ) ;

	    PrintProcessVector( process_vec ) ;
	    PrintProcessMap( initial_queue ) ;

    // Load input and store all processes in pending queue
	// PID, arriving time, each CPU burst, # of bursts, I/O time
	Process A('A' , 1 , 10 , 1 , 5 );
    	Process B('B' , 1 , 9 , 1 , 20 );
    	Process C('C' , 1 , 8 , 1 , 5 );
	Process D('D' , 7 , 2 , 1 , 5 );
	std::vector<Process*> tmp1, tmp2;
	tmp1.push_back(&A);
	tmp1.push_back(&B);
	tmp1.push_back(&C);
	tmp2.push_back(&D);
	std::map<int, std::vector<Process*> > pending_queue;
	pending_queue[1] = tmp1;
	pending_queue[7] = tmp2;

	ReadyQueue ready_queue;
	CPU core;
	Clock c;
	IO IOblocker;
	std::string mode("SRT");
	bool new_arrival = false;

	// Not considering IO queue
	while (!(ready_queue.empty() && core.idle() && pending_queue.empty())
		|| core.cs_block()) {
		// READY QUEUE UPDATE ------------------------------------
		// New processes arrive
		if (pending_queue.find(c.time()) != pending_queue.end()) {
			ready_queue.push(pending_queue[c.time()]);
			pending_queue.erase(c.time());
			ready_queue.sort(mode);
			new_arrival = true;
		}
		// Processes finish IO

		// CPU UPDATE --------------------------------------------
		core.tick();
		// Not during context switch
		if (!core.cs_block()) {
			// Load process into CPU
			if (core.idle() && !ready_queue.empty()) {
				core.push(ready_queue.front());
				ready_queue.pop();
			}
			// Remove totally completed process from CPU
			else if (!core.idle() && core.complete()) {
				IOblocker.push(core.current_process());
				core.pop();
			}
			// Remove completed process from CPU
			else if (!core.idle() && core.single_complete() && !ready_queue.empty()) {
				IOblocker.push(core.current_process());
				core.pop();
			}
			// Preempt process (RR)
			else if (!core.idle() && mode == "RR" 
				&& core.slice_over() && !ready_queue.empty()) {
				core.pop(); // should add process into Ready queue
			}
			// Preempt process (SRT)
			else if (new_arrival && !core.idle() && mode == "SRT" 
				&& ready_queue.front()->tCPU() < core.current_process()->tCPU()) {
				core.pop(); // should add process into Ready queue
			}
			else {
				core.run();
			}
			new_arrival = false;		
		}

		// IO QUEUE UPDATE -----------------------------------------
		IOblocker.tick();		

		// CLOCK UPDATE -----------------------------------------
		// time increases by 1 ms
		if (core.cs_block())
			std::cout << "time: " << c.time() << " CPU: " <<" switching";
		else if (!core.idle())
			std::cout << "time: " << c.time() << " CPU: "<<" process: " << core.current_processID();
		else
			std::cout << "time: " << c.time() << " CPU:  idle ";		
		std::cout <<" IO: "<<IOblocker.PIDs()<<std::endl;
		c.tick();
	}
	
	// Added functions for pending_qeueu by Feng
	int CountCharacter(std::string str)
{
    int count = 0 ;
    unsigned int i ;
    for( i = 0 ; i < str.size() ; i++ ) {
        if ( !isspace( str[ i ] ) )
            count++ ;
    }
    return count ;
}


int ReadProcesses( const char * file_name , ProcessVector & process_vec )
{
    std::ifstream ifs( file_name ) ;
    int proc_count = 0 ;

    if (ifs.is_open()) {
        std::string str;
        while (std::getline(ifs, str))
        {
            // skip comment line, blank line, and line with only whitespace
            if( str[ 0 ] == '#' || CountCharacter( str ) == 0 ) continue ;

            // read process information
            else
            {
                std::vector<std::string> process_info ;
                std::string delimiter = "|" ;
                size_t delim_pos = 0 ;
                std::string token ;
                int count_info = 0 ;
                
                // Seperate process parameters delimiter '|' 
                while ( ( delim_pos = str.find( delimiter ) ) != std::string::npos ) 
                {
                    count_info++ ;
                    token = str.substr(0, delim_pos) ;
                    process_info.push_back( token ) ;
                    str.erase( 0, delim_pos + delimiter.length() );
                }
                process_info.push_back( str ) ; // last parameter, t_io
                count_info++ ;
                
                process_vec.push_back
                    ( 
                      new Process
                      (       
                       process_info[ 0 ][ 0 ] , 
                       std::atoi( process_info[ 1 ].c_str() ) , 
                       std::atoi( process_info[ 2 ].c_str() ) , 
                       std::atoi( process_info[ 3 ].c_str() ) , 
                       std::atoi( process_info[ 4 ].c_str() ) 
                      )
                    ) ;
            }
            proc_count++ ;
        }
    }

    else {
        std::cout << "Cannot open file " << file_name << "\n" ;
        return EXIT_FAILURE ;
    }

    return proc_count ;
}


void ConvertProcessVecToMap( const ProcessVector & process_vec , ProcessMap & initial_queue )
{
    for( unsigned int i = 0 ; i < process_vec.size() ; i++ ){
        ProcessMap :: iterator itr ;

        // check whether there is process vector with the same arrival time
        int curr_arrival =  process_vec[ i ] -> arrival() ;
        itr = initial_queue.find( curr_arrival );
        if (itr != initial_queue.end())
            itr->second.push_back( process_vec[ i ] ) ;

        // process with unique arrival time
        else {
            ProcessVector proc_vec_tmp ;
            proc_vec_tmp.push_back( process_vec[ i ] ) ;
            initial_queue.insert( std::make_pair( process_vec[ i ] -> arrival() , proc_vec_tmp ) );
        }

    }
}


void PrintProcessVector( const ProcessVector & process_vec )
{ 
        std::cout << "*******************************************************************\n" ;
        std::cout << "Number of processes is " << process_vec.size() << "\n" ;
        std::cout << std::setw( 7 ) << "PID" 
                  << std::setw( 15 ) << "t_arrival" 
                  << std::setw( 15 ) << "t_burst"
                  << std::setw( 15 ) << "c_burst" 
                  << std::setw( 15 ) << "t_IO" 
                  << "\n" ;

        ProcessVector::const_iterator iter ;
        for(iter = process_vec.begin(); iter != process_vec.end(); iter++) {
            std::cout << std::setw( 7 ) <<  ( *iter )-> ID() 
                      << std::setw( 15 ) << ( *iter ) -> arrival() 
                      << std::setw( 15 ) << ( *iter ) -> burst_time()
                      << std::setw( 15 ) << ( *iter ) -> burst_num() 
                      << std::setw( 15 ) << ( *iter ) -> tIO() 
                      << "\n" ;
        }
        std::cout << "*******************************************************************\n" ;
}

void PrintProcessMap( const ProcessMap & initial_queue )
{
    ProcessMap::const_iterator itr ;
    for ( itr = initial_queue.begin(); itr != initial_queue.end(); itr++ ) {
        std::cout << "\n" << "Key value = " << itr -> first << '\n';
        PrintProcessVector( itr -> second ) ;
    }
}
	//Added end by Feng
	
	return EXIT_SUCCESS;
}
