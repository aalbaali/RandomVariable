#include <vector>
#include <string>

// Class of measurements. Contains i) measurement, ii) covariance, and iii) time of the measurement.
// Change the name of the class to RandomVariable (it just includes the mean, covariance, and time step)
template<size_t _MeanRows, size_t _MeanCols = 1, size_t dof = _MeanRows,typename T = double>
class RandomVariable{
    public:
        RandomVariable(){
            _mean = Eigen::Matrix< T, _MeanRows, _MeanCols>::Zero();
            _cov  = Eigen::Matrix< T, dof, dof>::Zero();
            _t    = -1.0;
        }

        RandomVariable( Eigen::Matrix< T, _MeanRows, _MeanCols> mean_in,
            Eigen::Matrix< T, dof, dof> cov_in = Eigen::Matrix< T, dof, dof>::Identity(), double time_in = -1){
            // Constructor that takes measurement and covariance (with default values).
            _mean = mean_in;
            _cov  = cov_in;
            _t    = time_in;
        }

        template<typename VectorT>
        RandomVariable( std::vector<VectorT> row_of_raw_data){
            double time = row_of_raw_data[ 0];
            Eigen::Matrix< double, _MeanRows, _MeanCols> mean;
            Eigen::Matrix< double, dof, dof> cov;
            for( size_t i = 0; i < _MeanRows; i++){
                mean( i) = row_of_raw_data[ i + 1];
                for( size_t j = 0; j < dof; j++){
                    // 1 : for time
                    // _MeanRows * _MeanCols : size of the mean
                    // i * Size : skipping each column of the matrix
                    cov( i, j) = row_of_raw_data[ 1 + _MeanRows * _MeanCols + i * dof + j];
                }
            }
            // Store objects
            _t    = time;
            _mean = mean;
            // Ensure symmetry of the covariance matrix
            _cov  =  0.5 * (cov + cov.transpose());
        }

        // Getters
        Eigen::Matrix< T, _MeanRows, _MeanCols> mean(){ return _mean;}
        Eigen::Matrix< T, dof, dof> cov(){ return  _cov;}
        double time(){ return _t;}

        // Setters
        void setMean( Eigen::Matrix< T, _MeanRows, _MeanCols> mean_in){
            this->_mean = mean_in;
        }

        void setCov( Eigen::Matrix<T, dof, dof> cov_in){
            this->_cov = cov_in;
        }
        void setTime( double time_in){ _t = time_in;}

    private:
        Eigen::Matrix< double, _MeanRows, _MeanCols> _mean;
        Eigen::Matrix< double, dof, dof> _cov;

        // Time of measurement
        double _t;
};
