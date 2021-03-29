// This is a header for the Input/Output of a random variable
#include <fstream> // Used to read file
#include <sstream> // Used for stringstream
#include <iomanip> // For nice outputs (setw)
#include <limits>  // Get the maximum number of digits for double precision

#include <vector>
#include <string>

// Width of each column
const size_t out_precision = std::numeric_limits< double >::max_digits10;
const size_t out_width     = out_precision + 5;

namespace RV{
    namespace IO{    
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
        std::vector< std::vector< T>> read( const std::string &file_name){
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

            // line that will track the file 
            std::string line;
            // Get first line (header)
            std::getline( infile, line);

            // Get number of columns
            const int num_cols = getNumberOfColumns( line);
            
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
        void WriteHeader(std::ostream& outstrm, size_t num_meas, std::string mean_symbol = "m"){
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
                    ss << mean_symbol << "_" << i + 1 << j + 1;
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
        template<typename T>
        void write(std::vector<T> meas_vec, std::vector<std::string> header_str, const std::string file_name){
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
            WriteHeader<T>( outstrm, meas_vec.size());
            // Write the header
            // for( auto h : header_str){
            //     outstrm << h << '\t';
            // }
            // outstrm << std::endl;
            
            // Now enter data
            for(auto rv : meas_vec){
                // Export time
                outstrm << std::left << std::setw(out_width) << std::setprecision(out_precision) << rv.time(); 
                
                // Write mean value (estimate)
                for(size_t i = 0; i < rv.MeanRows(); ++i){
                    for(size_t j = 0; j < rv.MeanCols(); ++j){
                        outstrm << std::setw(out_width) << std::setprecision(out_precision) << rv.mean()(i, j);
                    }
                }
                // Write covariance (column major)
                for(size_t j = 0; j < rv.Dof(); j++){
                    for(size_t i = 0; i < rv.Dof(); i++){
                        outstrm << std::setw(out_width) << std::setprecision(out_precision) << rv.cov()(i,j);
                    }
                }

                outstrm << std::setw(out_width) << rv.covIsGlobal();
                
                // Flush
                outstrm << std::endl;
            }        
        }

        template<typename T>
        std::vector< T> import(const std::string &file_name){
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
        void print(T rv){
            std::cout << std::left << std::setw(out_width) << rv.time();
            std::cout << std::setw(out_width) << rv.mean().transpose();
            // Vectorize covariance matrix
            Eigen::Map<Eigen::RowVectorXd> cov_vec( rv.cov().data(), rv.cov().size());
            std::cout << std::setw(out_width) << cov_vec;
        }
    }
}