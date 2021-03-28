#include <iostream>
#include <string>
#include <vector>

#include "Eigen/Dense"

#include "RV.h"

const std::string filename_in  = "/home/aa/Documents/Data/Data_generator/SE2/meas_vel.txt";
const std::string filename_out = "../test_out.txt";

// Random variable class
//      Measurement in
typedef RV::RandomVariable< 2> MeasVel;
//      Value out
const size_t dof_out = 2;   // Degrees of freedom of the output RV (this will be used as the size of the covariance matrix)
const size_t cols_out = 2;  // Number of colums of the mean of the output RV
const size_t rows_out = 1;  // Number of rows of the mean of the output RV
typedef Eigen::Matrix<double, cols_out, 1> MeanOut;
typedef Eigen::Matrix<double, dof_out, dof_out> CovOut;
typedef RV::RandomVariable< dof_out> RvMeanOut;

int main(int argc, char *argv[]){    
    // Import measurements
    std::vector< MeasVel> meas_in = RV::IO::import< MeasVel>( filename_in);

    // Generate measurements
    //  Number of measurements
    const unsigned int K = 100;
    std::vector< RvMeanOut> meas_out( K);
    for( size_t k = 0; k < K; ++k){
        meas_out[k].setTime( k);
        // Generate measurements
        meas_out[k].setMean( MeanOut( k, 0));

        // Covariance
        CovOut cov_k = CovOut::Random();
        cov_k = 0.5 * (cov_k + cov_k.transpose());
        meas_out[k].setCov( cov_k);
    }    


    // Output measurements
    // Definer header
    std::vector<std::string> header( dof_out * (dof_out + 1) + 1);
    // First entry: time
    header[0] = "Time";
    // Second inputs: states
    for(size_t i = 0; i < dof_out; i++){
        std::stringstream ss;
        ss << "x_" << i+1;
        ss >> header[1 + i];
    }
    // Third input: covariances
    for(size_t j = 0; j < dof_out; j++){
        for(size_t i = 0; i < dof_out; i++){
            std::stringstream ss;
            ss << "cov_" << (i+1) << (j+1);
            ss >> header[1 + dof_out + dof_out * j + i];
        }
    }
    RV::IO::write( meas_out, header, filename_out);
}