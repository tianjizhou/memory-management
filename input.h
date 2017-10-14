#ifndef INPUT_H_
#define INPUT_H_

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>  // std::pair, std::make_pair
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
        int ReadProcesses( const char * file_name , ProcessVector & process_vec ) ;
        void PrintProcessVector( const ProcessVector & process_vec ) ;
        void ConvertProcessVecToMap( const ProcessVector & process_vec , ProcessMap & initial_queue ) ;
        void PrintProcessMap( const ProcessMap & initial_queue ) ;
} ;

#endif  // INITIAL_QUEUE_H__
