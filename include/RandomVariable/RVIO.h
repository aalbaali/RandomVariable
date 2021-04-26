// This is a header for the Input/Output of a random variable
// 
// TODO:
//      1. Remove the templated argument. Everything should be read from the file immediately.
//      2. Separate function implementations into a separate C++ file. Use it as a (static) library if possible or as a precompiled header
// 
// Amro Al-Baali
// 31-Mar-2021

#ifndef RVIO_H
#define RVIO_H

#include <fstream> // Used to read file
#include <sstream> // Used for stringstream
#include <iomanip> // For nice outputs (setw)
#include <limits>  // Get the maximum number of digits for double precision
#include <tuple>

#include <vector>
#include <string>

// Width of each column
const size_t out_precision = std::numeric_limits< double >::max_digits10  - 2;
const size_t out_width     = out_precision + 10; // Requires at least a padding of 5 for scientific notation (i.e., the 'e-05' notation)

namespace RV{
    namespace IO{            
        static std::tuple< int, int> getMeanSize( std::string& str){
            // Gets the size of the mean element (column matrix or matrix) and returns a tuple of integers. Example of an input:
            //"mean_size		:	2,	1"
            
            // Stringstream
            std::stringstream ss;
            // Find the index of the colon
            size_t idx_col = str.find(':');
            // Truncate everything before the colon
            str = str.substr(idx_col + 1);

            // Sizes to be returned    
            int sz1, sz2;
            char c;
            ss.str( str);
            ss >> sz1 >> c >> sz2;
            return std::make_tuple( sz1, sz2);
        }

        static int getDof( std::string &str){
            // Gets the degrees of freedom (dof) of the random variable
            // Stringstream
            std::stringstream ss;
            // Find the index of the colon
            size_t idx_col = str.find(':');
            // Truncate everything before the colon
            str = str.substr(idx_col + 2);
            ss.str( str);
            // Sizes to be returned    
            int dof;    
            ss >> dof;
            return dof;
        }
         
        static int getNumMeas( std::string &str){
            // Gets the number of measurements

            // Gets the degrees of freedom (dof) of the random variable
            // Stringstream
            std::stringstream ss;
            // Find the index of the colon
            size_t idx_col = str.find(':');
            // Truncate everything before the colon
            str = str.substr(idx_col + 2);
            ss.str( str);
            // Sizes to be returned    
            int num_meas;    
            ss >> num_meas;
            return num_meas;
        }
        static int getNumberOfColumns( std::string &header){
            // Gets the number of columns from the header
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
        static std::vector< std::vector< T>> read( const std::string &file_name){
            // This function reads the data from the text file. The first line is a header file and will be used to count the number of columns. E.g., for a random variable of dimension 2, the header may look like
            //  Tim     x_1     x_2     cov_11  cov_21  cov_12  cov_22.
            // 
            // @params[in] const std::string file_name
            //      Full file path to read data from.
            
            // Open file stream
            std::ifstream infile( file_name);

            // Check if file is open
            if (!infile.is_open())
                perror("error while opening file");

            // *********************************
            // Header
            // line that will track the file 
            std::string line;
            
            // 1. mean_size
            std::getline( infile, line);
            size_t msz1, msz2;
            std::tie( msz1, msz2) = getMeanSize( line);

            // 2. dof (degrees of freedom)
            std::getline( infile, line);
            size_t dof = getDof( line);
            
            // 3. num_meas (number of measurements)
            std::getline( infile, line);
            size_t num_meas = getNumMeas( line);

            // 4. Line breaker
            std::getline( infile, line);

            // 5. Column titles
            std::getline( infile, line);
            // Get number of columns
            const int num_cols = getNumberOfColumns( line);
            
            // 6. Line breaker
            std::getline( infile, line);

            // Create a (dynamic) vector that includes a vector of size num_cols
            std::vector< std::vector <T> > data( num_meas);

            // Go over data and store
            for( int i = 0; getline( infile, line); i++){
                // Assign line to string stream
                std::stringstream ss( line);
                
                // Data at the current row
                std::vector< T> data_row (num_cols);        

                for( int j = 0; ss.good(); j++){
                    std::string substr;
                    getline( ss, substr, ',' );
                    std::stringstream ss2( substr);
                    ss2 >> data_row[j];
                }
                // Store row vector
                data[i] = data_row;
            }

            infile.close();

            return data;
        }

        template<typename T>
        static void WriteHeader(std::ostream& outstrm, size_t num_meas, std::string mean_symbol){
            // This is a templated function that returns the header string. The string contains:
            //  mean_size : T::MeanRows(), T::MeanRows() 
            //      Size of the mean element
            //  dof       : T::Dof()    
            //      Degrees of freedom of the random variable
            //  num_meas  : num_meas_in
            //      Number of measurements
            // The column headers will be of the form
            //
            //  Time m_11 m_21 m_31 m_12 m_22 m_32 ... cov_11 cov_21 cov_31 cov_12 ... cov_33 covIsGlobal
            // Note that the "m" can be replaced by another symbol (default is "m").

            // Output mean_size
            outstrm << std::left << std::setw(out_width) << "mean_size" << ":\t" << T::MeanRows() << ",\t" << T::MeanCols() << std::endl;

            // Output degrees of freedom
            outstrm << std::left << std::setw(out_width) << "dof" << ":\t" << T::Dof() << std::endl;

            // Output number of measurements
            outstrm << std::left << std::setw(out_width) << "num_meas" << ":\t" << num_meas << std::endl;

            // Create a separater
            outstrm << std::string( out_width * (T::MeanRows() * T::MeanCols() + std::pow(T::Dof(), 2) + 2), '=') << std::endl;
            
            // Now, the column headers
            //  First, the time
            outstrm << std::left << std::setw(out_width) << "Time";
            //  Second, the mean elements
            for(size_t i = 0; i < T::MeanRows(); ++i){
                for( size_t j = 0; j < T::MeanCols(); ++j){
                    std::ostringstream ss;
                    ss << mean_symbol << "_" << j + 1 << i + 1;
                    outstrm  << std::left << std::setw(out_width) << ss.str() << std::right;
                }
            }

            // Third, the covariance header
            for(size_t i = 0; i < T::Dof(); ++i){
                for( size_t j = 0; j < T::Dof(); ++j){
                    std::ostringstream ss;
                    ss << "cov_" << i + 1 << j + 1;
                    outstrm << std::left << std::setw(out_width) << ss.str();
                }
            }
            // Finally, output the covariance type
            outstrm << std::left << std::setw(out_width) << "covIsGlobal" << std::endl;
            
            // Create a separater
            outstrm << std::string( out_width * (T::MeanRows() * T::MeanCols() + std::pow(T::Dof(), 2) + 2), '=') << std::endl;
        }

        // Make this static/const
        template<typename T>
        static void exportEntity(std::ostream &outstrm, T entity){
            std::ostringstream oss;
                oss <<  entity << ",";
                outstrm << std::setw(out_width) << oss.str();
        }
        template<typename T>
        static void write(std::vector<T> meas_vec, const std::string file_name, std::string mean_symbol = "m"){
            // A function that writes the data to a text file of the appropriate format.
            // 
            // @params[in] meas_vec
            //      A vector of random variables (an object of class RandomVariable). This class has a `mean()' field and `cov()' field.
            // @param[in] header_str
            //      A vector of header titles.
            // @param[in] file_name
            //      Full path to the .txt file to be exported.

            std::ofstream outstrm(file_name);
            // Write header
            WriteHeader<T>( outstrm, meas_vec.size(), mean_symbol);
            
            // Now enter data
            outstrm << std::fixed << std::left;
            // outstrm << std::fixed;
            for(auto rv : meas_vec){
                // Export time
                exportEntity( outstrm, rv.time());

                // Write mean value (estimate)
                for(size_t i = 0; i < T::MeanRows(); ++i){
                    for(size_t j = 0; j < T::MeanCols(); ++j){
                        exportEntity( outstrm, rv.mean()(i, j));                        
                    }
                }
                // Write covariance (column major)
                for(size_t j = 0; j < T::Dof(); j++){
                    for(size_t i = 0; i < T::Dof(); i++){
                        exportEntity( outstrm, rv.cov()(i,j));
                    }
                }
                exportEntity( outstrm, rv.covIsGlobal());
                
                // Flush
                outstrm << std::endl;
            }        
        }

        template<typename T>
        static std::vector< T> import(const std::string &file_name){
            // Import raw vector data
            auto raw_data = read( file_name);
            // Vector of measurement objects
            std::vector< T> measurements( raw_data.size());
            for( int i = 0; i < raw_data.size(); i++){
                // meas_control_input[ i] = getMeasurementObject< MeasControlInput, size_u>( raw_data[i]);
                measurements[ i] = T( raw_data[i]);
            }

            return measurements;
        }

        // Function that displays the random variable
        template<typename T>
        static void print(T rv){
            std::cout << std::left << std::setw(out_width) << rv.time();
            std::cout << std::setw(out_width) << rv.mean().transpose();
            // Vectorize covariance matrix
            Eigen::Map<Eigen::RowVectorXd> cov_vec( rv.cov().data(), rv.cov().size());
            std::cout << std::setw(out_width) << cov_vec;
        }
    }
}

#endif