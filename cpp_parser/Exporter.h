#include <string>
#include <fstream>

namespace RandomVariable{
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