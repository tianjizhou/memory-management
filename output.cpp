// FILE: output.cpp
#include "output.h"

void Output::PrintStart( const std::string & algorithm , const int & t ) {

    std::string o_alg ;
    if ( algorithm == "next" ) 
        o_alg = "(Contiguous -- Next-Fit)" ;

    else if ( algorithm == "first" ) 
        o_alg = "(Contiguous -- First-Fit)" ;

    else if ( algorithm == "best" ) 
        o_alg = "(Contiguous -- Best-Fit)" ;

    else if ( algorithm == "non" ) 
        o_alg = "(Non-contiguous)" ;

    else {
        std::cout << "Please input one of the four modes:\n\t1.\"next\"  2.\"first\"  3.\"best\"  4.\"non\"\n" ;
        std::cerr << "ERROR: Output::PrintStart() Invalid algorithm\n" ; 
        exit( EXIT_FAILURE ) ;
    }

    std::cout << "time " << t << "ms: " << "Simulator started " << o_alg << std::endl ;
}

void Output::PrintArrive( const int & t , const char & pid , const int & frame_num ){

    std::cout << "time " << t << "ms: " << "Process " << pid <<" arrived (requires "<< frame_num << " frames)" 
              << std::endl ;
} 

void Output::PrintPlace( const int & t , const char & pid ) {

    std::cout << "time " << t << "ms: Placed " << "process " << pid << ":" << std::endl ;
} 

void Output::PrintRemove( const int & t , const char & pid ) {

    std::cout << "time " << t << "ms: " << "Process " << pid << " removed:" << std::endl ;
} 

// state = -1, not enough space, skip; state = 1, enough space for defragmentation
void Output::PrintDefragBegin( const int & t , const char & pid , const int & state ) {

    if( state == 1)
        std::cout << "time " << t << "ms: " << "Cannot place process " << pid << " -- starting defragmentation" << std::endl ;

    else if ( state == -1 )
        std::cout << "time " << t << "ms: " << "Cannot place process " << pid << " -- skipped!"<< std::endl ;

    else {
        std::cout << "Please input one of the below two values:\n\t1.\"-1\"  2.\"1\"\n" ;
        std::cerr << "ERROR: Output::PrintDefragBegin() Invalid state\n" ; 
        exit( EXIT_FAILURE ) ;
    }
} 

void Output::PrintDefragEnd( const int & t , const std::vector< char > & frag_pids , const int & frame_num ) {

    std::cout << "time " << t << "ms: " << "Defragmentation complete (moved " << frame_num << " frames: " ;

    for ( std::vector<char>::const_iterator itr = frag_pids.begin(); itr != frag_pids.end(); ++itr ) {
        if( ( itr + 1 )  == frag_pids.end() )
            std::cout << *itr ;
        else
            std::cout << *itr << ", ";
    }

    std::cout << ")"<< std::endl ;
} 

void Output::PrintEnd( const std::string & algorithm , const int & t ) {

    std::string o_alg ;
    if ( algorithm == "next" ) 
        o_alg = "(Contiguous -- Next-Fit)" ;

    else if ( algorithm == "first" ) 
        o_alg = "(Contiguous -- First-Fit)" ;

    else if ( algorithm == "best" ) 
        o_alg = "(Contiguous -- Best-Fit)" ;

    else if ( algorithm == "non" ) 
        o_alg = "(Non-contiguous)" ;

    else {
        std::cout << "Please input one of the four modes:\n\t1.\"next\"  2.\"first\"  3.\"best\"  4.\"non\"\n" ;
        std::cerr << "ERROR: Output::PrintEnd() Invalid algorithm\n" ; 
        exit( EXIT_FAILURE ) ;
    }

    std::cout << "time " << t << "ms: " << "Simulator ended " << o_alg << std::endl ;
}

void Output::PrintPageVector( const std::vector< int > & page_frame ) {

    std::vector< int > :: const_iterator iter ;
    int i = 0 ;
    for( iter = page_frame.begin() ; iter != page_frame.end() ; iter++ ) {
        //if( ( i + 1 ) % 10 == 0)
        if( i % 10 == 0 && i != 0 )
            std::cout << std::endl ;
        else
            std::cout << " " ;
        std::cout << "["<< i << "," << *iter << "]" ;
        i++ ;
    }
    std::cout << std::endl ;
} 

void Output::PrintPageTable( const std::map< char, std::vector< int > > & page_table ) {

    std::cout << "PAGE TABLE [page,frame]:\n" ;
    std::map< char, std::vector< int > >::const_iterator iter ;
    for( iter = page_table.begin() ; iter != page_table.end() ; iter++ ) {
        std::cout << iter -> first << ":" ;
        PrintPageVector( iter -> second ) ;
    }
} 
