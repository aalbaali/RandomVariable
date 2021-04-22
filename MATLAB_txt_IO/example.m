%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   Importing KF estimates and plotting error plots.
%
%   Amro Al Baali
%   15-Mar-2021
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% clear all;
close all;

%% Load data
% Ground truth file name
file_name_gt = "\\wsl$\Ubuntu-20.04\home\aa\Documents\Data\Data_generator\SE2\gt_states.txt";
% State estimates file name
file_name_est= "\\wsl$\Ubuntu-20.04\home\aa\Documents\Data\Data_generator\SE2\est_xhat.txt";

% Load data
%   Specify the number of columns since the ground truth doesn't include
%   covariances
struct_gt  = importTextFile( file_name_gt);
struct_est = importTextFile( file_name_est);

% Plotting range
idx_range = 1 : min( length(struct_gt.time), length( struct_est.time));

% Re-normalize poses lambda function
renormalize = @(X_k) se2alg.expMap( se2alg.vee( SE2.logMap( X_k)));
X_hat = struct_est.values3d;
X_gt  = struct_gt.values3d;
for kk = 1 : length( idx_range)
    C_k = X_hat( 1 : 2, 1 : 2, kk);
    C_k = SO2.synthesize( atan2( C_k(2, 1), C_k(1, 1)));
    X_hat( 1 : 2, 1 : 2, kk) = C_k;
    
    C_k = X_gt( 1 : 2, 1 : 2, kk);
    C_k = SO2.synthesize( atan2( C_k(2, 1), C_k(1, 1)));
    X_gt( 1 : 2, 1 : 2, kk) = C_k;
end
%% Plot
% Lambda function to compute error
SE2Ominus = @(X_k1, X_k2) se2alg.vee(SE2.logMap( X_k1 \ X_k2));
% Compute errors
dxi = cell2mat( arrayfun(@(kk) SE2Ominus( X_gt(:,:,kk), ...
    X_hat(:,:,kk)), idx_range, 'UniformOutput', false));
% Get covariance matrices
time = struct_gt.time;
figure; 

P = struct_est.cov;
% Convert from local to global covariances
P = reshape( cell2mat( arrayfun(@(kk) SE2.adjoint( X_hat(:, :, kk)) * P( :, :, kk)...
    * SE2.adjoint( X_hat( :, :, kk))', idx_range, 'UniformOutput', false)), 3, 3, []);
for kk = 1 : 3
    subplot(3, 1, kk);
    hold all; grid on;
    plot( time', dxi(kk,:), 'LineWidth', 1.5);
    plot(  time, 3 * sqrt( squeeze( P( kk, kk, idx_range))), '-.r',...
        'LineWidth', 1.5);
    plot(  time, -3 * sqrt( squeeze( P( kk, kk, idx_range))), '-.r',...
        'LineWidth', 1.5);
    ylabel( sprintf('$\\delta \\xi_{%i}$ [m]', kk),...
                    'Interpreter', 'latex', 'FontSize', 14);
end
xlabel( '$t_{k}$ [s]', 'Interpreter', 'latex', 'FontSize', 14);
