%% PROBLEM 02
% Simulate the packet information flowing in a router working under TCP/IP
% connection and evaluate the result
% ---------------------------------------------------------------------------
clear all
close all
clc

%% DATA GIVEN
% ---------------------------------------------------------------------------
s = tf('s');
tol = 1e-3;
c = 3750;                               % Link capacity (packets/s)
N = 60;                                 % Load factor (number of TCP sessions)
R = 0.246;                              % Round Trip Time RTT (s)
l = 1.86e-4;                            % Controller plant parameter
k = 5e-3;                               % Controller plant parameter
q = ((c^2)/(2*N)) * exp(-s*R);          % Queue length (packets)
p = (s + (2*N)/(c*R^2)) * (s + 1/R);    % Probability of packet mark/drop
G = zpk(minreal(q/p, tol));             % Transfer function of the system
Kc = 9.64e-6;                           % Controller plant parameter
z = 0.53;                               % Controller plant parameter

% Several AQM algorithms are available, but the one that has received special attention 
% is the random early detection (RED) algorithm. The RED algorithm dynamics can be approximated 
% through the linear controller:

%C = (l) / (1 + s/k);  % VERSION 1
C = (Kc*(1 + s/z)) / (s);

%% TUNING THE SIMULINK MODEL
% ---------------------------------------------------------------------------
rho = 1;                                % Amplitude of the step input reference
delta_a = 0;                            % Amplitude of the step input disturbance
delta_y = 0;                            % Amplitude of the step output disturbance
stop_time = 200;

%% RUNNING THE SIMULATIONM
% ---------------------------------------------------------------------------
out = sim('problem_02_sim');

%% PLOTTING THE RESULTS
% ----------------------------------------------------------
figure(1)
hold on
plot(out.y.time, out.y.data, 'r')
plot(out.r.time, out.r.data, 'b')
legend('Output signal', 'Reference signal')
title('Comparing signals')
grid on

figure(2)
plot(out.e.time, out.e.data, 'r')
title('Tracking error')
grid on

figure(3)
plot(out.u.time, out.u.data, 'r')
title('Control input')
grid on

%% COMPUTING THE RESULTS
% ----------------------------------------------------------
T = (C*G)/(1 + C*G);
res_T = stepinfo(T, 'SettlingTimeThreshold', 0.01, 'RiseTimeLimits', [0.1, 0.9])
disp('Steady State tracking error value: ');
disp(1/(1 + dcgain(C*G)))