%% PROBLEM 02
% Given a second order system transfer function, evaluate the natural
% frequency, the damping coefficient and the time constant of the poles.
% Then evaluate the steady state value, the maximum overshoot and peak
% time, the rise time and the 5% settling time
% ------------------------------------------------------------------------
clear all
close all
clc

%% GIVEN DATA
% ------------------------------------------------------------------------
s = tf('s');
H = 10 / (s^2 + 1.6*s + 4);

%% EVALUATE THE NATURAL FREQUENCY AND THE DAMPING RATIO
% Use the function damp() that, given a particular system, it returns the
% natural frequency and the damping ration of that system
% ------------------------------------------------------------------------
[wn, zeta] = damp(H);
tau = 1/(wn(1)*zeta(1));

%% COMPUTE THE OTHER RESULTS
% ------------------------------------------------------------------------
ss_value = dcgain(H);                             
peak_time = pi/(wn(1)*sqrt(1-zeta(1)^2));
maximum_overshoot = exp((-pi*zeta(1)) / sqrt(1-zeta(1)^2));
rise_time = (1 / (wn(1)*sqrt(1-zeta(1)^2))) * (pi - acos(zeta(1)));
settling_time = 3/(wn(1)*zeta(1));

%% ALTERNATIVE SOLUTION
% ------------------------------------------------------------------------
S = stepinfo(H, 'SettlingTimeThreshold', 0.05, 'RiseTimeLimits', [0, 1])


%% PLOT THE STEP RESPONSE
% Use the step() function to obtain the plot of the step response
% ------------------------------------------------------------------------
figure(1)
step(H, 'r')
title('Step response of the system')
xlabel('Time [t]')
ylabel('Step response of the system u(t)')
grid on

%% SHOWING RESULTS
% ------------------------------------------------------------------------
disp('The natural frequency is ')
disp(wn(1))
disp('The damping ratio is ')
disp(zeta(1))
disp('The time constant is ')
disp(tau)
disp('The steady state value is ')
disp(ss_value)
disp('The maximum overshoot is ')
disp(maximum_overshoot)
disp('The peak time is ')
disp(peak_time)
disp('The rise time is ')
disp(rise_time)
disp('The settling time is ')
disp(settling_time)