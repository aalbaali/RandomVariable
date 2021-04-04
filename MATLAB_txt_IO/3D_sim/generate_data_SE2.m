%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   DECAR's data generator will be used to generate the ground truth file. This
%   ground truth file will in turn be used to generate measurement data.
%
%   Velocity measurement is generated instead of accelerometer. No bias.
%
%   Amro Al-Baali
%   22-Mar-2021
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all;

gt_filename = 'G:\My Drive\Professional\Code_base\Local\MATLAB\Research_codebase\DECAR\sim_generator\Output\gt_states.mat';
gt_struct = load( gt_filename, 'gt_states');
gt_states = gt_struct.gt_states;

%% Paths
% Add paths
addpath( '..');
% Export location
dir_out = '\\wsl$\Ubuntu-20.04\home\aa\Documents\Data\Data_generator\SE2';
%% Simulation parameters
% Dimension of the problem (2)
dim_x = 2;
% Degrees of freedom
dof_x = 3;
% Data generator time
t_gt = gt_states.time;
% Ground truth simulation frequency
f_gt = 1 / (t_gt(2) - t_gt(1));
% (Downsampled) simulation frequency
f_sim = f_gt; % [Hz] (could also be lower than this value)

% Interoceptive
%   Velocity sensor
%       Frequency
f_vel    = f_sim;
%       Noise covariance
cov_vel  = 0.01^2 * eye( dim_x);
%       Index of first measurement
idx_vel_0  = 1;

%   Gyro 
%       Frequency
f_gyro   = f_sim;
%       Noise covariance
cov_gyro = (0.005)^2 * eye( dof_x - dim_x); % [rad/s];
%       Index of first measurement
idx_gyro_0 = 1;

% Exteroceptive
%   GPS 
%       Frequency
f_gps    = 5;
%       Noise covariance
cov_gps  = 0.01^2 * eye( dim_x);
%       Index of first measurement
idx_gps_0 = f_sim / f_gps;

%% Get data
% Velocity
%   Indices
idx_vel = idx_vel_0 : (f_gt / f_vel) : length( t_gt);
%   Time
t_vel = t_gt( idx_vel);
% Gyro
%   Indices
idx_gyro = idx_gyro_0 : ( f_gt / f_gyro) : length( t_gt);
%   Time
t_gyro = t_gt( idx_gyro);
% GPS 
%   Indices
idx_gps = idx_gps_0 : ( f_gt / f_gps) : length( t_gt);
%   Time
t_gps  = t_gt( idx_gps);

%% Get (corrupted) measurements
% Velocity
%   True measurement
meas_vel = gt_states.v_btt_b( 1 : dim_x, idx_vel);
%   Corrupted
meas_vel = meas_vel + chol( cov_vel)' * randn( size( meas_vel));

% Gyro
%   True measurement
meas_gyro = gt_states.omega_bi_b( 3, idx_gyro);
%   Corrupteed
meas_gyro = meas_gyro + chol( cov_gyro)' * randn( size( meas_gyro));

% GPS
%   True measurement
meas_gps = gt_states.r_bt_t( 1 : dim_x, idx_gps);
%   Corrupted
meas_gps = meas_gps + chol( cov_gps)' * randn( size( meas_gps));

%% Generate files
disp('Genearting velocity sensor file');
tic();
% Velocity sensor
%   Convert covariances to row matrix 
cov_vel_3d = repmat( cov_vel, 1, 1, length( idx_vel));
%   Generate text file
generateTextFile( fullfile(dir_out, 'meas_vel.txt'), t_vel, meas_vel, ...
    cov_vel_3d, 'v_btt_b');    
toc();

% Gyro sensor
disp('Genearting gyro sensor file');
tic();
%   Convert covariances to row matrix 
cov_gyro_3d = repmat( cov_gyro, 1, 1, length( idx_gyro));
%   Generate text file
generateTextFile( fullfile(dir_out, 'meas_gyro.txt'), t_gyro, meas_gyro, ...
    cov_gyro_3d, 'omega_bi_b');    
toc();

% Position sensor
disp('Genearting GPS sensor file');
tic();
%   Convert covariances to row matrix 
cov_gps_3d = repmat( cov_gps, 1, 1, length( idx_gps));
%   Generate text file
generateTextFile( fullfile(dir_out, 'meas_gps.txt'), t_gps, meas_gps, ...
    cov_gps_3d, 'r_bt_t');
toc();