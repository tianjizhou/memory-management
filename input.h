// =======================================
// Team members:
//
// Jinghua Feng 	fengj3	
// Jihui Nie		niej
// Tianji Zhou		zhout2
//
// github link: https://github.com/tianjizhou/operating-system/
// =======================================

// FILE: input.h
#ifndef INPUT_H_
#define INPUT_H_

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>  // std::setw
#include <ctype.h>

#include "process.h"

typedef std::vector< Process * > ProcessVector ;
typedef std::map< int , std::vector< Process * > > ProcessMap ;

class Input
{
    public:

        Input(){} ;
        int CountCharacter( std::string str ) ;
        bool StringIsNumber( const std::string str ) ;
        bool ValidProcess( const std::vector< std:: string > process_info ) ;
        bool ReadProcesses( const char * file_name , ProcessVector & process_vec, int& num_processes, int& burst_t) ;
        void PrintProcessVector( const ProcessVector & process_vec ) ;
        void ConvertProcessVecToMap( const ProcessVector & process_vec , ProcessMap & initial_queue ) ;
        void PrintProcessMap( const ProcessMap & initial_queue ) ;
} ;

#endif  // INITIAL_QUEUE_H__
