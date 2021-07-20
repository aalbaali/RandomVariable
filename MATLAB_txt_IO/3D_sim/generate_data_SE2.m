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

% gt_filename = '\\wsl$\Ubuntu-20.04\home\aa\Documents\Data\Data_generator\SE2\gt_states.mat';
gt_filename = 'G:\My Drive\Professional\McGill\Masters\Data\2G\Simulation\figure_8_trajectory_lessnoisy\gt_states.mat';
gt_struct = load( gt_filename, 'gt_states');
gt_states = gt_struct.gt_states;

% Generate LCs
generate_lcs = true;
% Save text files
save_text_files = false;
% Save .mat files
save_mat_files = true;
% Number of trials
num_trials = 50;

rng('default');
%% Paths
% Add paths
addpath( '..');
% Export location
% dir_out = '\\wsl$\Ubuntu-20.04\home\aa\Documents\Data\Data_generator\SE2';
% dir_out = 'G:\My Drive\Professional\McGill\Masters\Data\2G\Simulation\figure_8_trajectory_lessnoisy';
dir_out = 'G:\My Drive\Professional\McGill\Masters\Data\2G\Simulation\figure_8_trajectory_lessnoisy_mct\noisy_data';
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
t_sim = gt_states.time;

% Prior
%   Covariance
cov_prior = (1e-3)^2 * eye( 3);

% Interoceptive
%   Velocity sensor
%       Frequency
f_vel    = f_sim;
%       Noise covariance
cov_vel  = 0.1^2 * eye( dim_x);
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
f_gps    = 1;
%       Noise covariance
cov_gps  = 0.01^2 * eye( dim_x);
%       Index of first measurement
idx_gps_0 = f_sim / f_gps + 1;

if generate_lcs
  %   Loop closures
  %       Noise covariance
  cov_lc   = 1e-5 * eye( 3);
end
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

if generate_lcs
  %   Loop closures (optional)
  %     Choose the LC crossing location (plot the ground-truth to choose a point)
  r_crossing = [ 5.016; 5.016];
  %     Find indices that cross this point
  r_gt = gt_states.r_bt_t( 1 : 2, :);
  %     Plot vecnorm( r_gt - r_crossing) and determine the threshold that the norm
  %     should be under
  [~, idx_crossings] = find( vecnorm( r_gt - r_crossing) < 0.03);
%   warning('Ignoring the first crossing');
%   idx_crossings(1) = [];
  %     Convert to loop closure indices (w.r.t. the first crossing)
  n_lc = length( idx_crossings) - 1;
  idx_lc = [ repmat( idx_crossings( 1), n_lc, 1), idx_crossings( 2 : end)'];
end

%% Get true measurements
% Prior
%   True measurement (renormalized)
meas_prior_true = SE2.synthesize( gt_states.C_bt(1 : 2, 1 : 2, 1)', gt_states.r_bt_t(1:2,1));
%   Ensure that it's SE(2) element
meas_prior_true = se2alg.expMap( se2alg.vee( SE2.logMap( meas_prior_true)));

% Velocity
%   True measurement
meas_vel_true = gt_states.v_btt_b( 1 : dim_x, idx_vel);

% Gyro
%   True measurement
meas_gyro_true = gt_states.omega_bi_b( 3, idx_gyro);

% GPS
%   True measurement
meas_gps_true = gt_states.r_bt_t( 1 : dim_x, idx_gps);

if generate_lcs
  % Loop closures
  %   Measurement model is of the form
  %     [-]_k = T_1 \ T_2
  meas_lc_true = nan( 3, 3, n_lc);
  for lv1 = 1 : n_lc
    % LC first index
    idx_1 = idx_lc( lv1, 1);
    idx_2 = idx_lc( lv1, 2);
    % Pose at first index
    T_1 = SE2.synthesize( gt_states.C_bt( 1 : 2, 1 : 2, idx_1)',  ...
      gt_states.r_bt_t( 1 : 2, idx_2));
    T_2 = SE2.synthesize( gt_states.C_bt( 1 : 2, 1 : 2, idx_2)',  ...
      gt_states.r_bt_t( 1 : 2, idx_2));
    % True measurement
    meas_lc_true( :, :, lv1) = T_1 \ T_2;    
  end
end

%% Corrupt measurements
for lv_trial = 1 : num_trials  
  fprintf('Corrupting data for trial\t%i\tof\t%i\n', lv_trial, num_trials);
  % Prior
  meas_prior = meas_prior_true * se2alg.expMap( chol( cov_prior)' * randn( 3, 1));

  % Velocity
  meas_vel = meas_vel_true + chol( cov_vel)' * randn( size( meas_vel_true));

  % Gyro
  meas_gyro = meas_gyro_true + chol( cov_gyro)' * randn( size( meas_gyro_true));

  % GPS
  meas_gps = meas_gps_true + chol( cov_gps)' * randn( size( meas_gps_true));

  if generate_lcs
    % Loop closures
    %   Measurement model is of the form
    %     [-]_k = T_1 \ T_2
    meas_lc = nan( 3, 3, n_lc);
    for lv1 = 1 : n_lc
      % LC first index
      idx_1 = idx_lc( lv1, 1);
      idx_2 = idx_lc( lv1, 2);
      % Pose at first index
      T_1 = SE2.synthesize( gt_states.C_bt( 1 : 2, 1 : 2, idx_1)',  ...
        gt_states.r_bt_t( 1 : 2, idx_2));
      T_2 = SE2.synthesize( gt_states.C_bt( 1 : 2, 1 : 2, idx_2)',  ...
        gt_states.r_bt_t( 1 : 2, idx_2));    
      % Corrupt measurment
      meas_lc( :, :, lv1) = meas_lc_true( :, :, lv1) * se2alg.expMap( chol( cov_lc)' * randn( 3, 1));
    end
  end

  %% Convert covariances sizes/shapes
  % Velocity sensor
  %  Convert covariances to row matrix 
  cov_vel_3d = repmat( cov_vel, 1, 1, length( idx_vel));
    
  %  Gyro
    cov_gyro_3d = repmat( cov_gyro, 1, 1, length( idx_gyro));
  
  %  GPS
  cov_gps_3d = repmat( cov_gps, 1, 1, length( idx_gps));
  
  %  Loop closures
  if generate_lcs
    cov_lc_3d = repmat( cov_lc, 1, 1, n_lc);
  end
  
  % Construct true poses
  X_poses = zeros( 3, 3, length( t_gt));
  for kk = 1 : length( t_gt)
    X_k = SE2.synthesize( gt_states.C_bt(1:2,1:2,kk)', gt_states.r_bt_t(1:2,kk));
    X_k = se2alg.expMap( se2alg.vee( SE2.logMap( X_k)));    
    X_poses(:,:, kk) = X_k;
  end
  %% Generate files
  if save_text_files
    disp('Genearting prior sensor file');
    tic();
    generateTextFile( fullfile(dir_out, 'meas_prior.txt'), t_vel(1), meas_prior, ...
        cov_prior, 'X', 3);
    toc();
    disp('Genearting velocity sensor file');
    tic();    
    generateTextFile( fullfile(dir_out, 'meas_vel.txt'), t_vel, meas_vel, ...
        cov_vel_3d, 'v_btt_b');    
    toc();

    Gyro sensor
    disp('Genearting gyro sensor file');
    tic();    
    %  Generate text file
    generateTextFile( fullfile(dir_out, 'meas_gyro.txt'), t_gyro, meas_gyro, ...
        cov_gyro_3d, 'omega_bi_b');    
    toc();

    Position sensor
    disp('Genearting GPS sensor file');
    tic();        
    %  Generate text file
    generateTextFile( fullfile(dir_out, 'meas_gps.txt'), t_gps, meas_gps, ...
        cov_gps_3d, 'r_bt_t');
    toc();

    if generate_lcs
      % LC "sensor"
      disp('Genearting GPS sensor file');
      tic();
      %   Generate text file
      generateTextFile( fullfile(dir_out, 'meas_lc.txt'), t_lc, meas_lc, ...
          cov_lc_3d, 'r_bt_t');
      toc();
    end

    disp('Generating ground truth file');
    tic();    
    generateTextFile( fullfile(dir_out, 'gt_states.txt'), t_gt, X_poses, ...
        [], 'X', 3);
    toc();
  end

  %% Create .mat file
  if save_mat_files
    % Simulation information
    data_struct.sim.time = t_gt;
    data_struct.sim.freq = f_sim;

    % Measurements
    %   Prior
    data_struct.meas.prior.mean = meas_prior;
    data_struct.meas.prior.cov  = cov_prior;
    data_struct.meas.prior.time = t_gt(1);
    %   Linear velocity
    data_struct.meas.velocity.mean = meas_vel;
    data_struct.meas.velocity.cov  = cov_vel_3d;
    data_struct.meas.velocity.time = t_vel;
    data_struct.meas.velocity.freq = f_vel;
    %   Angular velocity
    data_struct.meas.gyro.mean = meas_gyro;
    data_struct.meas.gyro.cov  = cov_gyro_3d;
    data_struct.meas.gyro.time = t_gyro;
    data_struct.meas.gyro.freq = f_gyro;
    %   GPS
    data_struct.meas.gps.mean = meas_gps;
    data_struct.meas.gps.cov  = cov_gps_3d;
    data_struct.meas.gps.time = t_gps;
    data_struct.meas.gps.freq = f_gps;
    if generate_lcs
      %   LCs
      data_struct.meas.lc.mean  = meas_lc;
      data_struct.meas.lc.cov   = cov_lc_3d;
      data_struct.meas.lc.idx   = idx_lc;
      data_struct.meas.lc.time  = t_sim( idx_lc);
    end

    % Noisy dat filename
    if num_trials == 1
      filename_noisy_data = 'noisy_data';
    else
      filename_noisy_data = sprintf('noisy_data_%02i', lv_trial);
    end
    % Save struct
    save( fullfile( dir_out, filename_noisy_data), 'data_struct');

    % Save ground truth (once)
    if lv_trial == 1
      save( fullfile( dir_out, 'gt_SE2'), 'X_poses');
    end    
  end  
end
fprintf( "Saved files to '%s'\n", dir_out);