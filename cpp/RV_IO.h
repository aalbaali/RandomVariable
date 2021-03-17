#include <fstream> // Used to read file
#include <sstream> // Used for stringstream
#include <vector>
#include <string>

namespace Prob{
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



    template<typename T>
    void LogMeasurements(std::vector<T> meas_vec, std::vector<std::string> header_str, const std::string file_name){
        // meas_vec is a vector of random variable objects. Each random variable object contains a time of measurement, mean value, and covariance
        std::ofstream outstrm(file_name);
        // Write the header
        for( auto h : header_str){
            outstrm << h << '\t';
        }
        outstrm << std::endl;
        // Now enter data
        for(auto rv : meas_vec){
            // Export time
            outstrm << rv.time() << '\t';
            
            // Write mean value (estimate)
            for(size_t i = 0; i < rv.meas().size(); i++){
                outstrm << rv.meas()(i) << '\t';
            }
            // Write covariance (column major)
            for(size_t j = 0; j < rv.meas().size(); j++){
                for(size_t i = 0; i < rv.meas().size(); i++){
                    outstrm << rv.cov()(i,j) << '\t';
                }
            }

            // Flush
            outstrm << std::endl;
        }        
    }
}
