#include <iostream>
#include "data_parser.h"
#include "Exporter.h"

int main(int argc, char *argv[]){
    std::string file_name = "/home/aalbaali/Documents/Code_base/Examples/Data_generator/linear_system/data/msd_ground_truth.txt";

    std::cout << "Filename: " << file_name << std::endl;
    auto data = importData( file_name);
    
    // ************************************************************
    // Go over data vector and display results
    std::cout << "Data:\n" << std::endl;
    for( auto i : data){
        for( auto j : i){
            std::cout << j << "\t";
        }
        std::cout << std::endl;
    }

}