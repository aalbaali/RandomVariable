#include <iostream>
#include <string>
#include <vector>

#include "Eigen/Dense"

#include "RV.h"
#include "RVIO.h"


// const std::string filename_in  = "/home/aa/Documents/Data/Data_generator/SE2/meas_vel.txt";
// const std::string filename_in  = "/home/aa/Documents/Data/Data_generator/linear_system/msd_acc.txt";
const std::string filename_in  = "/home/aa/Documents/Data/Data_generator/linear_system/msd_pos.txt";
const std::string filename_out = "../test_out.txt";

// Random variable class
//      Measurement in
typedef RandomVariable< 1> MeasVel;
//      Value out
const size_t dof_out = 3;   // Degrees of freedom of the output RV (this will be used as the size of the covariance matrix)
const size_t cols_out = 3;  // Number of colums of the mean of the output RV
const size_t rows_out = 3;  // Number of rows of the mean of the output RV
typedef Eigen::Matrix<double, rows_out, cols_out>       MeanOut;
typedef Eigen::Matrix<double, dof_out, dof_out>         CovOut;
typedef RandomVariable< rows_out, cols_out, dof_out>    RvMeanOut;


//TEMP
#include <array>

int main(int argc, char *argv[]){    
    // Import measurements
    std::vector< MeasVel> meas_in = RV::IO::import< MeasVel>( filename_in);

    for(auto i : meas_in){
        std::cout << i.time() << "\t" << i.mean() << "\t" << i.cov().transpose() << std::endl;
    }
    // Generate measurements
    //  Number of measurements
    const unsigned int K = 100;
    std::vector< RvMeanOut> meas_out( K);
    for( size_t k = 0; k < K; ++k){
        meas_out[k].setTime( k);
        // Generate measurements
        meas_out[k].setMean( MeanOut::Random());

        // Covariance
        CovOut cov_k = CovOut::Random();
        cov_k = 0.5 * (cov_k + cov_k.transpose());
        meas_out[k].setCov( cov_k);
    }    


    // Output measurements
    RV::IO::write( meas_out, filename_out, "x");
}

// #include <iostream>
// #include <string>
// #include <sstream>
// #include <tuple>
// #include "Eigen/Dense"

// #include "RVIO.h"


// std::string str = "0.080000,        0.003114,        0.076769,       ";


// int main(){
//     std::stringstream ss( str);
//     while( ss.good() )
//     {
//         std::string substr;
//         getline( ss, substr, ',' );
//         double x;
//         std::stringstream ss2( substr);
//         ss2 >> x;
//         std::cout << x << std::endl;
//     }

//     // std::cout << RV::IO::getNumMeas( str) << std::endl;
// }