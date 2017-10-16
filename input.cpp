// FILE: input.cpp
#include "input.h"

int Input::CountCharacter( std::string str )
{
	int count = 0 ;
	unsigned int i ;
	for( i = 0 ; i < str.size() ; i++ ) {
		if ( !isspace( str[ i ] ) )
		    count++ ;
	}
	return count ;
}

bool Input::StringIsNumber( const std::string str ) {
    /*
    for ( unsigned int i = 0 ; i < str_vec.size() ; i++ ) {
        if( str_vec[ i ].find_first_not_of( "0123456789" ) != string::npos )
            return false ; 
    }
    return true ;
    */
    return str.find_first_not_of( "0123456789" ) == std::string::npos ;


}

bool Input::ValidProcess( const std::vector< std:: string > process_info )
{
    std::locale loc ;
    char pid = process_info[ 0 ][ 0 ] ;
    if( !( 
           
           process_info.size() == 5 && 
           std::isupper( pid , loc ) && 
           StringIsNumber( process_info[ 1 ] ) &&
           StringIsNumber( process_info[ 2 ] ) && 
           StringIsNumber( process_info[ 3 ] ) && 
           StringIsNumber( process_info[ 4 ] )
         ) 
      )
        return false ;

    return true ;
}


bool Input::ReadProcesses( const char * file_name , ProcessVector & process_vec, int& num_processes, int& burst_t)
{
    std::ifstream ifs( file_name ) ;
    num_processes = 0 ;
    burst_t = 0;

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
                //str = str.substr( 0, str.length() - 1 ) ; // remove '\n'
                str = str.substr( 0, str.length() ) ; // remove '\n'
                process_info.push_back( str ) ; // last parameter, t_io


                // check whether input format is valid
                if ( ValidProcess( process_info) == false ) {
                    std::cerr << "ERROR: Invalid input file format\n" ;
                    return EXIT_FAILURE ;
                }
                    
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
                burst_t += std::atoi(process_info[2].c_str())*std::atoi(process_info[3].c_str());
            }
            num_processes++ ;
        }
    }

    else {
        std::cout << "Cannot open file " << file_name << "\n" ;
        return EXIT_FAILURE ;
    }
    return EXIT_SUCCESS;
}


void Input::ConvertProcessVecToMap( const ProcessVector & process_vec , ProcessMap & initial_queue )
{
    for( unsigned int i = 0 ; i < process_vec.size() ; i++ ){
        ProcessMap :: iterator itr ;

        // check whether there is process vector with the same arrival time
        int curr_arrival =  process_vec[ i ] -> initial_arrival_time() ;
        itr = initial_queue.find( curr_arrival );
        if (itr != initial_queue.end())
            itr->second.push_back( process_vec[ i ] ) ;

        // process with unique arrival time
        else {
            ProcessVector proc_vec_tmp ;
            proc_vec_tmp.push_back( process_vec[ i ] ) ;
            initial_queue.insert( std::make_pair( process_vec[ i ] -> initial_arrival_time() , proc_vec_tmp ) );
        }

    }
}


void Input::PrintProcessVector( const ProcessVector & process_vec )
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
                      << std::setw( 15 ) << ( *iter ) -> initial_arrival_time() 
                      << std::setw( 15 ) << ( *iter ) -> burst_time()
                      << std::setw( 15 ) << ( *iter ) -> burst_num() 
                      << std::setw( 15 ) << ( *iter ) -> tIO() 
                      << "\n" ;
        }
        std::cout << "*******************************************************************\n" ;
}

void Input::PrintProcessMap( const ProcessMap & initial_queue )
{
    ProcessMap::const_iterator itr ;
    for ( itr = initial_queue.begin(); itr != initial_queue.end(); itr++ ) {
        std::cout << "\n" << "Key value = " << itr -> first << '\n';
        PrintProcessVector( itr -> second ) ;
    }
}
