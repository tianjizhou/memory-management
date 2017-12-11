// =======================================
// Team members:
//
// Jinghua Feng 	fengj3	
// Jihui Nie		niej
// Tianji Zhou		zhout2
//
// github: https://github.com/tianjizhou/memory-management
// =======================================

// FILE: output.h
#ifndef OUTPUT_H_
#define OUTPUT_H_ 

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

class Output
{
    public:
        Output(){} ;
        void PrintStart( const std::string & algorithm , const int & t ) ; 
        void PrintArrive( const int & t , const char & pid , const int & frame_num ) ; 
        void PrintPlace( const int & t , const char & pid ) ; 
        void PrintRemove( const int & t , const char & pid ) ; 
        void PrintDefragBegin( const int & t , const char & pid , const int & state ) ; 
        void PrintDefragEnd( const int & t , const std::vector< char > & frag_pids , const int & frame_num ) ; 
        void PrintEnd( const std::string & algorithm , const int & t ) ; 
        void PrintPageVector( const std::vector< int > & page_frame ) ; 
        void PrintPageTable( const std::map<char, std::vector<int> > & page_table ) ;
} ;

#endif  // OUTPUT_H_ 
