#pragma once
#include <fstream> // Used to read file
#include <sstream> // Used for stringstream
#include <vector>

int getNumberOfColumns( std::string &header){
    std::stringstream ss( header);
    int i = 0;
    // String that stores the column/variable name
    std::string col_name;
    while( ss >> col_name){
        // Just count number of columns for now        
        i++;
    }
    return i;
}

template< typename T = double>
std::vector< std::vector< T>> importData( const std::string &file_name){
    // Open file stream
    std::ifstream infile( file_name);

    // Check if file is open
    if (!infile.is_open())
        perror("error while opening file");

    // line that will track the file 
    std::string line;
    // Get first line (header)
    std::getline( infile, line);

    // Get number of columns
    const int num_cols = getNumberOfColumns( line);
#ifdef DEBUG
    std::cout << "Number of columns:\t" << num_cols << std::endl;
#endif
    
    // Create a (dynamic) vector that includes a vector of size num_cols
    std::vector< std::vector <T> > data;

    // Go over data and store
    for( int i = 0; std::getline( infile, line); i++){
        // Assign line to string stream
        std::stringstream ss( line);
        
        // Data at the current row
        std::vector< T> data_row (num_cols);        
        for( int j = 0; ss >> data_row[j]; j++){
            // Do nothing (it's already assigned)
        }
        // Store row vector
        data.push_back( data_row);
    }

    infile.close();

    return data;
}
