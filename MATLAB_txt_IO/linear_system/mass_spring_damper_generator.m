%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   Mass-spring-damper data generator. Generates a text file that includes
%       -   System parameters
%       -   Input array
%       -   Correction array
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all;
close all;

% For repeatability
rng( 'default');
addpath('..');
% Data directories (exporting locations)
data_dir = "/home/aalbaali/Documents/Data/Data_generator/linear_system";
%% Simulation parameters
% Simulation frequency
f_sim = 100;
% Simulation duration
t_final = 10; % [s]
% Initial conditions
x0 = [ 0; 0];

%% System parameters
% Mass
sys_m = 1;
% Spring
sys_k = 1;
% Damping
sys_b = 1;

% Build the continuous-time (CT) system matrices
A_ct = [ 0, 1; -sys_k / sys_m, -sys_b / sys_m];
B_ct = [ 0; 1 / sys_m];

% Frequency of input
f_u = 0.1; % [Hz]
% Function of a system input
func_u = @( t) cos( 2 * pi * f_u * t);

% Correction frequency
f_y = 1;
% First correction time
t_y_0 = 0.5; %1 / f_y;
% Correction matrix
C = [ 1, 0];
%% Noise properties
% Variance on input
var_u = 1e0;

% Variance on correction
var_y = 1e-1;

%% Get true data (solve ODE)
% Process model
process_model = @( t, x) A_ct * x + B_ct * func_u( t);
% Solve ODE
[ t, x_true] = ode45( process_model, ( 0 : 1 / f_sim : t_final), x0);
% Transpose solution
x_true = x_true';
t      = t';

%% Generate TRUE data
u_true = arrayfun( @( t_k) func_u( t_k), t);

% Get the correction time at correction time steps
t_y = t_y_0 : ( 1 / f_y) : t_final;
% Get the indices at which correction occurs;
if rem( t_y_0 * f_sim, 10) ~= 0
    warning(' First correction time step not a multiple of the time steps');
end
idx_y = t_y_0 * f_sim : ( f_sim / f_y) : length( t);
% Get the true exteroceptive measurements
y_true = C * x_true( : , idx_y);

%% Corrupt data by noise
u_noisy = u_true + sqrt( var_u) * randn( size( u_true));
y_noisy = y_true + sqrt( var_y) * randn( size( y_true));

%% Plot ground truth
figure;
subplot( 2, 1, 1); hold all; grid on;
plot( t, x_true( 1, :), 'LineWidth', 1.5, 'DisplayName', 'True position');
% Plot true corrective measurement (it measures position)
scatter( t_y, y_true, 100, 'x', 'LineWidth', 1.5, 'DisplayName', 'True  corrections');
scatter( t_y, y_noisy, 100, 'x', 'LineWidth', 1.5, 'DisplayName', 'Noisy corrections');
ylabel( '$x(t_{k})$ [m]', 'Interpreter', 'latex', 'FontSize', 14);
legend( 'interpreter', 'latex', 'fontsize', 10);

subplot( 2, 1, 2); hold all; grid on;
plot( t, x_true( 2, :), 'LineWidth', 1.5, 'DisplayName', 'True velocity');
ylabel( '$\dot{x}(t_{k})$ [m/s]', 'Interpreter', 'latex', 'FontSize', 14);
xlabel( '$t_{k}$ [s]', 'Interpreter', 'latex', 'FontSize', 14);


%% Generate text file
% Interoceptive measurements
generateTextFile( t, [ u_noisy; repelem( var_u, 1, length( t))], ...
    { 'u_1', 'var_u1'}, fullfile(data_dir, 'msd_acc.txt'));

generateTextFile( t_y, [ y_noisy; repelem( var_y, 1, length( t_y))], ...
    { 'y_1', 'var_y1'}, fullfile(data_dir, 'msd_pos.txt'));

% Ground truth
generateTextFile( t, x_true, ...
    { 'x_1', 'x_2'}, fullfile( data_dir, 'msd_ground_truth.txt'));