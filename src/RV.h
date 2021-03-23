#include <fstream> // Used to read file
#include <sstream> // Used for stringstream
#include <vector>
#include <string>
#include <iomanip> // For nice outputs (setw)


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

    // Class of measurements. Contains i) measurement, ii) covariance, and iii) time of the measurement.
    // Change the name of the class to RandomVariable (it just includes the mean, covariance, and time step)
    template<size_t Size, typename T = double>
    class RandomVariable{
        public:
            RandomVariable(){
                _mean = Eigen::Matrix< T, Size, 1>::Zero();
                _cov  = Eigen::Matrix< T, Size, Size>::Zero();
                _t    = -1.0;
            }

            RandomVariable( Eigen::Matrix< T, Size, 1> mean_in,
                Eigen::Matrix< T, Size, Size> cov_in = Eigen::Matrix< T, Size, Size>::Identity(), double time_in = -1){
                // Constructor that takes measurement and covariance (with default values).
                _mean = mean_in;
                _cov  = cov_in;
                _t    = time_in;
            }

            template<typename VectorT>
            RandomVariable( std::vector<VectorT> row_of_raw_data){
                double time = row_of_raw_data[ 0];
                Eigen::Matrix< double, Size, 1> mean;
                Eigen::Matrix< double, Size, Size> cov;
                for( size_t i = 0; i < Size; i++){
                    mean( i) = row_of_raw_data[ i + 1];
                    for( size_t j = 0; j < Size; j++){
                        cov( i, j) = row_of_raw_data[ 1 + Size + i * Size + j];
                    }
                }
                // Store objects
                _t    = time;
                _mean = mean;
                // Ensure symmetry of the covariance matrix
                _cov  =  0.5 * (cov + cov.transpose());
            }

            // Getters
            Eigen::Matrix< T, Size, 1> mean(){ return _mean;}
            Eigen::Matrix< T, Size, Size> cov(){ return  _cov;}
            double time(){ return _t;}

            // Setters
            void setMean( Eigen::Matrix< T, Size, 1> mean_in){
                this->_mean = mean_in;
            }

            void setCov( Eigen::Matrix<T, Size, 2> cov_in){
                this->_cov = cov_in;
            }
            void setTime( double time_in){ _t = time_in;}

        private:
            Eigen::Matrix< double, Size, 1> _mean;
            Eigen::Matrix< double, Size, Size> _cov;

            // Time of measurement
            double _t;
    };
}
