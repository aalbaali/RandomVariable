#ifndef RV_H
#define RV_H

#include <vector>
#include <string>
#include "Eigen/Dense"

// Class of measurements. Contains i) measurement, ii) covariance, and iii) time of the measurement.
// Change the name of the class to RandomVariable (it just includes the mean, covariance, and time step)
template<size_t _MeanRows, size_t _MeanCols = 1, size_t _dof = _MeanRows,typename _T = double>
class RandomVariable{
    public:
        RandomVariable(){
            _mean = Eigen::Matrix< _T, _MeanRows, _MeanCols>::Zero();
            _cov  = Eigen::Matrix< _T, _dof, _dof>::Zero();
            _t    = -1.0;
        }

        RandomVariable( Eigen::Matrix< _T, _MeanRows, _MeanCols> mean_in,
            Eigen::Matrix< _T, _dof, _dof> cov_in = Eigen::Matrix< _T, _dof, _dof>::Identity(), bool cov_is_global_in = true, double time_in = -1){
            // Constructor that takes measurement and covariance (with default values).
            _mean           = mean_in;
            _cov            = cov_in;
            _cov_is_global  = cov_is_global_in;
            _t              = time_in;
        }

        template<typename VectorT>
        RandomVariable( std::vector<VectorT> row_of_raw_data){
            // Creates an object from a vector of vectors (usually obtained from `read` IO function (raw data))
            double time = row_of_raw_data[ 0];
            Eigen::Matrix< double, _MeanRows, _MeanCols> mean;
            Eigen::Matrix< double, _dof, _dof> cov;
            for( size_t i = 0; i < _MeanRows; i++){
                for( size_t j = 0; j < _MeanCols; j++){
                    mean( i, j) = row_of_raw_data[ i * _MeanRows + j + 1];
                }
                for( size_t j = 0; j < _dof; j++){
                    // 1 : for time
                    // _MeanRows * _MeanCols : size of the mean
                    // i * Size : skipping each column of the matrix
                    cov( i, j) = row_of_raw_data[ 1 + _MeanRows * _MeanCols + i * _dof + j];
                }
            }
            // Store objects
            _t    = time;
            _mean = mean;
            // Ensure symmetry of the covariance matrix
            _cov  =  0.5 * (cov + cov.transpose());
        }

        // Getters
        Eigen::Matrix< _T, _MeanRows, _MeanCols> mean(){ return _mean;}
        Eigen::Matrix< _T, _dof, _dof> cov(){ return  _cov;}
        double time(){ return _t;}
        bool covIsGlobal(){ return _cov_is_global;}
        // Setters
        void setMean( Eigen::Matrix< _T, _MeanRows, _MeanCols> mean_in){
            this->_mean = mean_in;
        }

        void setCov( Eigen::Matrix<_T, _dof, _dof> cov_in){
            this->_cov = cov_in;
        }

        void setTime( double time_in){ _t = time_in;}

        // Set covariance type (global/local). This is relavent to manifold random varaibles. Default is true (for RVs on the Euclidean space)
        void setCovIsGlobal( bool cov_is_global_in){
            this->_cov_is_global = cov_is_global_in;
        }
        // Get the template parameters (doesn't occupy space)        
        static constexpr size_t MeanRows()  noexcept{
            return _MeanRows;
        }
        static constexpr size_t MeanCols() noexcept{ 
                return _MeanCols;
        }
        static constexpr size_t Dof()      noexcept{ 
                return _dof;
        }
    private:                
        Eigen::Matrix< double, _MeanRows, _MeanCols> _mean;
        Eigen::Matrix< double, _dof, _dof> _cov;

        // Time of measurement
        double _t;

        // Global/local covariance (relavent to manifold RVs)
        bool _cov_is_global = true;
    public:
};

#endif