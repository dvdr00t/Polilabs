%% PROBLEM 03
% Given a certain step response, derive the transfer function by computing
% the natural frequency and the damping ratio coefficient
% ------------------------------------------------------------------------

clear all
close all
clc

%% GIVEN DATA
% ------------------------------------------------------------------------
s = tf('s');
u_step = 5;         % Given
ss_value = 1;       % Evaluated from the plot
peak_value = 1.17;  % Evaluated from the plot
peak_time = 0.45;   % Evaluated from the plot

%% COMPUTING RESULTS
% ------------------------------------------------------------------------
K = ss_value / u_step;
max_overshoot = (peak_value - ss_value) / ss_value; 
zeta = sqrt((log(max_overshoot)^2)/((log(max_overshoot))^2 + pi^2));
wn = pi / (peak_time * sqrt(1 - zeta^2));

%% SHOWING RESULTS
% ------------------------------------------------------------------------
H = (K * wn^2) / (s^2 + 2*zeta*wn*s + wn^2);
opt = stepDataOptions('InputOffset', 0, 'StepAmplitude', u_step);
figure(1)
step(H, 'r', opt)
title('Step response of the system with input amplitude 5')
xlabel('Time [t]')
ylabel('Step response')
grid on