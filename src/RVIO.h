// This is a header for the Input/Output of a random variable
#include <fstream> // Used to read file
#include <sstream> // Used for stringstream
#include <iomanip> // For nice outputs (setw)

#include <vector>
#include <string>


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
                for(size_t i = 0; i < rv.mean().size(); i++){
                    outstrm << rv.mean()(i) << '\t';
                }
                // Write covariance (column major)
                for(size_t j = 0; j < rv.mean().size(); j++){
                    for(size_t i = 0; i < rv.mean().size(); i++){
                        outstrm << rv.cov()(i,j) << '\t';
                    }
                }

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
            std::cout << std::setw(1) << rv.time() << "\t\t";
            std::cout << std::setw(5) << rv.mean().transpose() << "\t\t";
            // Vectorize covariance matrix
            Eigen::Map<Eigen::RowVectorXd> cov_vec( rv.cov().data(), rv.cov().size());
            std::cout << std::setw(5) << cov_vec;
        }
    }
}