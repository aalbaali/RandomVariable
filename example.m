%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   Importing KF estimates and plotting error plots.
%
%   Amro Al Baali
%   15-Mar-2021
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all;
close all;

%% Load data
% Ground truth file name
file_name_gt = "/home/aalbaali/Documents/Data/Data_generator/linear_system/msd_ground_truth.txt";
% State estimates file name
file_name_est = "/home/aalbaali/Documents/Data/Data_generator/linear_system/msd_kf_estimates.txt";

% Load data
%   Specify the number of columns since the ground truth doesn't include
%   covariances
struct_gt  = importTextFile( file_name_gt,  2);
struct_est = importTextFile( file_name_est, 2);

%% Plot
% Plotting range
idx_range = 1 : min( length(struct_gt.time), length( struct_est.time));
x_hat = struct_est.values( :, idx_range);
x_gt  = struct_gt.values ( :, idx_range);
time  = struct_est.time( idx_range);

figure; 
for kk = 1 : 2
    subplot(2, 1, kk);
    hold all; grid on;
    plot( time', x_hat(kk,:) - x_gt(kk,:), 'LineWidth', 1.5);
    plot(  time, 3 * sqrt( squeeze( struct_est.cov( kk, kk, idx_range))), '-.r',...
        'LineWidth', 1.5);
    plot(  time, -3 * sqrt( squeeze( struct_est.cov( kk, kk, idx_range))), '-.r',...
        'LineWidth', 1.5);
end
