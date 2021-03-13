#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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

int main(){
    const char* file_name = "/home/aalbaali/Documents/Code_base/Examples/Data_generator/linear_system/data/msd_ground_truth.txt";

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
    std::cout << "Number of columns:\t" << num_cols << std::endl;
    // // String stream to parse the string line
    // std::stringstream ss;

    // ss.str( line);
    // std::string var_name;
    // while( ss){
    //     ss >> var_name;
    //     std::cout << var_name << "\n";
    // }
    // std::cout << std::endl;

    

    infile.close();
}