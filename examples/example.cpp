#include <iostream>
#include <string>
#include <vector>

#include "Eigen/Dense"
#include "manif/SE2.h"

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

// Manif derived class
class SE2RV : private RvMeanOut{
    public:
        SE2RV(): RvMeanOut(){};      

        SE2RV(manif::SE2d & SE2_obj_in)
            : RvMeanOut(SE2_obj.transform()){
            SE2_obj = SE2_obj_in;
            this->_mean = SE2_obj_in.transform();
        };

        Eigen::Matrix< double, rows_out, cols_out> mean(){ return _mean;}

        void setMean( Eigen::Matrix< double, rows_out, cols_out> mean_in){            
            RvMeanOut::setMean(mean_in);
            this->SE2_obj.transform() = mean_in;
            SE2_obj = manif::SE2d( mean_in(0, 2), mean_in(1, 2), mean_in(0, 0), mean_in(1,0));
        };

        manif::SE2d se2(){ return SE2_obj;};

    private:
        // Manif derived object.
        manif::SE2d SE2_obj;
};


//TEMP
#include <array>

int main(int argc, char *argv[]){    
    // // Import measurements
    // std::vector< MeasVel> meas_in = RV::IO::import< MeasVel>( filename_in);

    // for(auto i : meas_in){
    //     std::cout << i.time() << "\t" << i.mean() << "\t" << i.cov().transpose() << std::endl;
    // }
    // // Generate measurements
    // //  Number of measurements
    // const unsigned int K = 100;
    // std::vector< RvMeanOut> meas_out( K);
    // for( size_t k = 0; k < K; ++k){
    //     meas_out[k].setTime( k);
    //     // Generate measurements
    //     meas_out[k].setMean( MeanOut::Random());

    //     // Covariance
    //     CovOut cov_k = CovOut::Random();
    //     cov_k = 0.5 * (cov_k + cov_k.transpose());
    //     meas_out[k].setCov( cov_k);
    // }    


    // // Output measurements
    // RV::IO::write( meas_out, filename_out, "x");

    manif::SE2d Xmanif;
    // Xd.setIdentity();
    Xmanif.setRandom();
    std::cout << Xmanif << std::endl;    
    SE2RV X( Xmanif);

    std::cout << X.mean() << std::endl;
    X.setMean( Eigen::Matrix3d::Identity());
    std::cout << X.se2() << std::endl;
}